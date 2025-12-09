#include <ORangeSessionHandler.hpp>
#include <SipStack.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <resip/dum/ServerInviteSession.hxx>

void ORangeSessionHandler::onNewSession(
    resip::ServerInviteSessionHandle h,
    resip::InviteSession::OfferAnswerType oat, const resip::SipMessage& msg) {

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
}