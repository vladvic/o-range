#include <ORangeSessionHandler.hpp>
#include <S2B/CommandBus.hpp>
#include <S2B/SignalCommand.hpp>
#include <SipStack.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <resip/dum/ClientInviteSession.hxx>
#include <resip/dum/ServerInviteSession.hxx>

void ORangeSessionHandler::onNewSession(
  resip::ServerInviteSessionHandle h,
  resip::InviteSession::OfferAnswerType oat,
  const resip::SipMessage& msg)
{
  auto cmd = std::make_unique<SignalCommand>(SignalEventType::CREATED);
  cmd->setCompletionToken();
  const resip::NameAddr& from = msg.header(resip::h_From);
  const resip::NameAddr& to = msg.header(resip::h_To);

  cmd->source() = from.uri().getAorNoReally().c_str();
  cmd->destination() = to.uri().getAorNoReally().c_str();
  CommandBus::instance().publish(std::move(cmd));
  /*
  boost::asio::co_spawn(
      SipStack::instance().getDUMIOContext(),
      [h]() mutable -> boost::asio::awaitable<void> {
        boost::asio::steady_timer timer(
            co_await boost::asio::this_coro::executor);

        timer.expires_after(std::chrono::milliseconds(500));
        co_await timer.async_wait(boost::asio::use_awaitable);
        if (h.isValid()) {
          h->reject(486);
        }
      },
      boost::asio::detached);
  */
}

void ORangeSessionHandler::onNewSession(
  resip::ClientInviteSessionHandle h,
  resip::InviteSession::OfferAnswerType oat,
  const resip::SipMessage& msg)
{
  std::cout << "Starting outgoing Session" << std::endl;
}

void ORangeSessionHandler::onProvisional(resip::ClientInviteSessionHandle h,
                                         const resip::SipMessage& msg)
{
  std::cout << "Got provisional " << msg << std::endl;
}

void ORangeSessionHandler::onOfferRequired(resip::InviteSessionHandle h,
                                           const resip::SipMessage& msg)
{
  std::cout << "Offer required for dialog " << std::endl;
}

void ORangeSessionHandler::onTerminated(
  resip::InviteSessionHandle h,
  InviteSessionHandler::TerminatedReason reason,
  const resip::SipMessage* related)
{
  std::cout << "Session terminated " << std::endl;
}
void ORangeSessionHandler::onFailure(resip::ClientInviteSessionHandle,
                                     const resip::SipMessage& msg)
{
  std::cout << "FUCKING FAILURE" << std::endl;
}
