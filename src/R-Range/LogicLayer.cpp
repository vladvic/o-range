/***************************************************
 * LogicLayer.cpp
 * Created on Mon, 15 Dec 2025 16:47:15 +0000 by vladimir
 *
 * $Author$
 * $Rev$
 * $Date$
 ***************************************************/

#include <boost/asio/co_spawn.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <util/type_traits.hpp>

#include "Call.hpp"
#include "Device.hpp"
#include "Leg.hpp"
#include "Logger/Logger.hpp"
#include "Number.hpp"
#include "ObjectArena.hpp"
#include "Script.hpp"
#include "ScriptKey.hpp"
#include "ScriptProvider.hpp"
#include "Session.hpp"

#include "LogicLayer.hpp"

void LogicLayer::stop()
{
  post( // Make sure all previous work is done before stop() is executed
    [this]() { m_ioct.stop(); });
}

LogicLayer::LogicLayer()
  : m_workGuard(boost::asio::make_work_guard(m_ioct))
  , m_signalSet(m_ioct)
{
  m_signalSet.add(SIGINT);
  m_signalSet.async_wait([this](boost::system::error_code ec, int num)
                         { stop(); });
}

void LogicLayer::setScriptProvider(ScriptProvider* provider)
{
  m_scriptProvider = provider;
}

boost::asio::awaitable<bool> LogicLayer::handleEventScript(
  const SignalCommand& cmd)
{
  auto script = co_await getEventScript(cmd);
  if (!script) {
    co_return false;
  }
  auto token = cmd.getCompletionToken();
  auto arena = ArenaLocator::locate((intptr_t)(token.get()));
  if (!arena) {
    if ((SignalEventType)cmd.type() == SignalEventType::CREATED) {
      arena = std::make_shared<ObjectArena>();
    }
    else {
      co_return false;
    }
  }
  co_return script->execute(*arena, cmd);
}

boost::asio::awaitable<std::shared_ptr<Script>> LogicLayer::getEventScript(
  const SignalCommand& cmd)
{
  if (!m_scriptProvider) {
    co_return std::make_shared<Script>();
  }
  ScriptKey key(cmd);
  auto script = co_await m_scriptProvider->asyncGet(key);
  co_await boost::asio::dispatch(m_ioct, boost::asio::use_awaitable);
  if (!script) {
    co_return std::make_shared<Script>();
  }
  co_return script;
}

template<>
void LogicLayer::process<SignalEventType::CREATED>(const SignalCommand& cmd)
{
  auto sessionId = cmd.getCompletionToken();
  auto media = cmd.media();
  auto from = cmd.source();
  auto to = cmd.destination();
  post(
    [sessionId,
     media = std::move(media),
     from = std::move(from),
     to = std::move(to)]() mutable
    {
      auto arena = ArenaLocator::locate((intptr_t)(sessionId.get()));
      if (!arena) {
        arena = std::make_shared<ObjectArena>();
      }
      /* TODO: Locate number */
      // auto number  = std::make_shared<Number>();
      auto device = std::make_shared<Device>(from);
      auto session = std::make_shared<Session>(
        std::make_unique<TokenSessionId>(sessionId), device);
      auto call = std::make_shared<Call>();
      auto leg = std::make_shared<Leg>();
      leg->setSession(session);
      call->setInitiatingLeg(leg);
      arena->add(device);
      arena->add(session);
      arena->add(call);
      arena->add(leg);
      /* TODO: Perform configuration step */
      auto makeCall =
        std::make_unique<SignalCommand>(SignalCommandType::CREATE);
      makeCall->source() = from;
      makeCall->destination() = to + ":5061";
      makeCall->setCompletionToken();
      LOG_INFO("Creating outbound call to {}", makeCall->destination()).show();
      auto outboundSessionId = makeCall->getCompletionToken();
      auto outboundDevice = std::make_shared<Device>(to);
      auto outboundSession = std::make_shared<Session>(
        std::make_unique<TokenSessionId>(outboundSessionId), outboundDevice);
      auto outboundLeg = std::make_shared<Leg>();
      outboundLeg->setSession(outboundSession);
      call->setOutboundLeg(outboundLeg);
      arena->add(outboundDevice);
      arena->add(outboundSession);
      arena->add(outboundLeg);
      CommandBus::instance().publish(std::move(makeCall));
    });
  auto progress = std::make_unique<SignalCommand>(SignalCommandType::PROGRESS);
  progress->setCompletionToken(sessionId);
  CommandBus::instance().publish(std::move(progress));
}

template<>
void LogicLayer::process<SignalEventType::ACCEPTED>(const SignalCommand& cmd)
{
  auto sessionId = cmd.getCompletionToken();
  post(
    [sessionId]() mutable
    {
      auto arena = ArenaLocator::locate((intptr_t)(sessionId.get()));
      if (!arena) {
        return;
      }
      auto session = arena->session((intptr_t)sessionId.get());
      auto leg = session->leg()->call()->leg();
      if (leg) {
        auto& id = dynamic_cast<const TokenSessionId&>(leg->session()->id());
        auto accept =
          std::make_unique<SignalCommand>(SignalCommandType::ACCEPT);
        accept->setCompletionToken(id.token());
        CommandBus::instance().publish(std::move(accept));
      }
    });
}

template<>
void LogicLayer::process<SignalEventType::PROGRESS>(const SignalCommand& cmd)
{
  auto completionToken = cmd.getCompletionToken();
}

template<>
void LogicLayer::process<SignalEventType::RINGING>(const SignalCommand& cmd)
{
  auto sessionId = cmd.getCompletionToken();
  post(
    [sessionId]() mutable
    {
      auto arena = ArenaLocator::locate((intptr_t)(sessionId.get()));
      if (!arena) {
        return;
      }
      auto session = arena->session((intptr_t)sessionId.get());
      auto leg = session->leg()->call()->leg();
      if (leg) {
        auto& id = dynamic_cast<const TokenSessionId&>(leg->session()->id());
        auto ringing =
          std::make_unique<SignalCommand>(SignalCommandType::RINGING);
        ringing->setCompletionToken(id.token());
        CommandBus::instance().publish(std::move(ringing));
      }
    });
}

template<>
void LogicLayer::process<SignalEventType::REJECTED>(const SignalCommand& cmd)
{
  auto sessionId = cmd.getCompletionToken();
  auto rejectReason = cmd.rejectReason();
  post(
    [sessionId, rejectReason]() mutable
    {
      auto arena = ArenaLocator::locate((intptr_t)(sessionId.get()));
      if (!arena) {
        return;
      }
      auto session = arena->session((intptr_t)sessionId.get());
      auto leg = session->leg()->call()->leg();
      if (leg) {
        auto& id = dynamic_cast<const TokenSessionId&>(leg->session()->id());
        auto reject =
          std::make_unique<SignalCommand>(SignalCommandType::REJECT);
        reject->setCompletionToken(id.token());
        reject->rejectReason() = rejectReason;
        CommandBus::instance().publish(std::move(reject));
      }
    });
}

template<>
void LogicLayer::process<SignalEventType::MODIFIED>(const SignalCommand& cmd)
{
  auto completionToken = cmd.getCompletionToken();
}

template<>
void LogicLayer::process<SignalEventType::TERMINATED>(const SignalCommand& cmd)
{
  auto sessionId = cmd.getCompletionToken();
  post(
    [sessionId]() mutable
    {
      auto arena = ArenaLocator::locate((intptr_t)(sessionId.get()));
      if (!arena) {
        return;
      }
      auto session = arena->session((intptr_t)sessionId.get());
      auto leg = session->leg()->call()->leg();
      if (leg) {
        auto& id = dynamic_cast<const TokenSessionId&>(leg->session()->id());
        auto terminate =
          std::make_unique<SignalCommand>(SignalCommandType::TERMINATE);
        terminate->setCompletionToken(id.token());
        CommandBus::instance().publish(std::move(terminate));
      }
    });
}

void LogicLayer::notify(const Command& cmd)
{
  if (cmd.hasEnumType<SignalEventType>()) {
    auto& signalCommand = dynamic_cast<const SignalCommand&>(cmd);
    LOG_INFO("Got command {}", (SignalEventType)signalCommand.type()).show();

    boost::asio::co_spawn(
      m_ioct,
      [this, signalCommand]() -> boost::asio::awaitable<void>
      {
        if (co_await handleEventScript(signalCommand)) {
          co_return;
        }

        switch ((SignalEventType)signalCommand.type()) {
        case SignalEventType::CREATED:
          process<SignalEventType::CREATED>(signalCommand);
          break;
        case SignalEventType::ACCEPTED:
          process<SignalEventType::ACCEPTED>(signalCommand);
          break;
        case SignalEventType::PROGRESS:
          process<SignalEventType::PROGRESS>(signalCommand);
          break;
        case SignalEventType::RINGING:
          process<SignalEventType::RINGING>(signalCommand);
          break;
        case SignalEventType::REJECTED:
          process<SignalEventType::REJECTED>(signalCommand);
          break;
        case SignalEventType::MODIFIED:
          process<SignalEventType::MODIFIED>(signalCommand);
          break;
        case SignalEventType::TERMINATED:
          process<SignalEventType::TERMINATED>(signalCommand);
        }
        co_return;
      },
      boost::asio::detached);
  }
}

void LogicLayer::init()
{
  CommandBus::instance().subscribe(util::enum_traits<SignalEventType>::all,
                                   shared_from_this());
}

void LogicLayer::run()
{
  m_ioct.run();
}
