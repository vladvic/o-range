/***************************************************
 * LogicLayer.hpp
 * Created on Mon, 15 Dec 2025 16:47:27 +0000 by vladimir
 *
 * $Author$
 * $Rev$
 * $Date$
 ***************************************************/
#pragma once

#include <S2B/CommandBus.hpp>
#include <S2B/SignalCommand.hpp>
#include <boost/asio.hpp>
#include <boost/asio/awaitable.hpp>
#include <util/Singleton.hpp>

#include "ArenaLocator.hpp"
#include "Session.hpp"

class ScriptProvider;
class Script;

class TokenSessionId : public SessionId
{
  CompletionToken m_token;

public:
  TokenSessionId(CompletionToken t)
    : m_token(t)
  {
  }
  intptr_t hash() const override { return (intptr_t)m_token.get(); }
  CompletionToken token() const { return m_token; }
};

class LogicLayer
  : public CommandBusSubscriber
  , public std::enable_shared_from_this<LogicLayer>
  , public util::Singleton<LogicLayer>
{
  boost::asio::io_context m_ioct;
  boost::asio::executor_work_guard<boost::asio::io_context::executor_type>
    m_workGuard;
  boost::asio::signal_set m_signalSet;

  ArenaLocator m_arenaLocator;
  ScriptProvider* m_scriptProvider{ nullptr };

  void stop();

  template<typename T>
  void post(T&& callback)
  {
    boost::asio::post(m_ioct, std::move(callback));
  }

  boost::asio::awaitable<std::shared_ptr<Script>> getEventScript(
    const SignalCommand& cmd);
  boost::asio::awaitable<bool> handleEventScript(const SignalCommand& cmd);

  template<SignalEventType T>
  void process(const SignalCommand& cmd);

public:
  LogicLayer();
  void setScriptProvider(ScriptProvider* provider);
  void notify(const Command& cmd) override;
  void init();
  void run();
};
