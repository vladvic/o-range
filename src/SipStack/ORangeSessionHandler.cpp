#include <ORangeSessionHandler.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <resip/dum/ServerInviteSession.hxx>

void ORangeSessionHandler::onNewSession(
    resip::ServerInviteSessionHandle h,
    resip::InviteSession::OfferAnswerType oat, const resip::SipMessage& msg) {
  std::cout << "Received new session with OfferAnswerType: "
            << static_cast<int>(oat) << std::endl;
  std::cout << "SIP Message: " << msg.brief() << std::endl;

  if (auto ioctx = m_DUMIOContext.lock()) {
    boost::asio::co_spawn(
        *ioctx,
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

  // Handle new incoming session here
}