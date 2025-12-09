#include <SipStack.hpp>
#include <boost/asio.hpp>
#include <resip/dum/DialogUsageManager.hxx>
#include <resip/dum/InviteSessionHandler.hxx>
#include <resip/stack/EventStackThread.hxx>
#include <resip/stack/SipStack.hxx>

SipStack::SipStack()
    : m_masterProfile(std::make_shared<resip::MasterProfile>()),
      m_pollGrp(resip::FdPollGrp::create()),
      m_intr(new resip::EventThreadInterruptor(*m_pollGrp)),
      m_stack(0, resip::DnsStub::EmptyNameserverList, m_intr, false, 0, 0,
              m_pollGrp),
      m_DUM(m_stack),
      m_stackThread(m_stack, *m_intr, *m_pollGrp) {
  m_stack.addTransport(resip::UDP, 5060);

  m_masterProfile->addSupportedMethod(resip::INVITE);
  m_masterProfile->setUserAgent("ORange PBX/0.1");

  m_DUM.setMasterProfile(m_masterProfile);
  m_DUM.setInviteSessionHandler(&m_sessionHandler);

  m_stackThread.run();

  startDUM();
}

SipStack::~SipStack() {
  m_running = false;
  if (m_DUMThread.joinable()) {
    m_DUMThread.join();
  }
  m_stackThread.shutdown();
  m_stackThread.join();
}

void SipStack::startDUM() {
  boost::asio::co_spawn(
      m_IOContext,
      [this]() -> boost::asio::awaitable<void> {
        boost::asio::steady_timer timer(
            co_await boost::asio::this_coro::executor);
        while (m_running) {
          timer.expires_after(std::chrono::milliseconds(50));
          co_await timer.async_wait(boost::asio::use_awaitable);
          while (m_DUM.process());
        }
      },
      boost::asio::detached);

  m_DUMThread = std::move(std::thread([this]() { m_IOContext.run(); }));
}