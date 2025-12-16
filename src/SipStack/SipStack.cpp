#include <CustomLogger.hpp>
#include <S2B/SignalCommand.hpp>
#include <SipStack.hpp>
#include <boost/asio.hpp>
#include <resip/dum/DialogUsageManager.hxx>
#include <resip/dum/InviteSessionHandler.hxx>
#include <resip/stack/EventStackThread.hxx>
#include <resip/stack/SipStack.hxx>
extern "C" {
#include <sys/eventfd.h>
}

SipStack::SipStack()
  : m_logger(std::make_unique<CustomLogger>())
  , m_masterProfile(std::make_shared<resip::MasterProfile>())
  , m_pollGrp(resip::FdPollGrp::create())
  , m_intr(new resip::EventThreadInterruptor(*m_pollGrp))
  , m_stack(0,
            resip::DnsStub::EmptyNameserverList,
            m_intr,
            false,
            0,
            0,
            m_pollGrp)
  , m_DUM(m_stack)
  , m_stackEventFd(::eventfd(0, EFD_NONBLOCK | EFD_SEMAPHORE))
  , m_stackThread(m_stack, *m_intr, *m_pollGrp, m_stackEventFd)
  , m_asio_eventFd(m_IOContext, m_stackEventFd)
  , m_timer(m_IOContext)
{
  m_stack.addTransport(resip::UDP, 5060);

  m_masterProfile->addSupportedMethod(resip::INVITE);
  m_masterProfile->setUserAgent("ORange PBX/0.1");

  m_DUM.setMasterProfile(m_masterProfile);
  m_DUM.setInviteSessionHandler(&m_sessionHandler);

  m_stackThread.run();

  startDUM();
}

SipStack::~SipStack()
{
  if (m_DUMThread.joinable()) {
    boost::asio::post(m_IOContext, [this]() { m_IOContext.stop(); });
    m_DUMThread.join();
  }
  m_stackThread.shutdown();
  m_stackThread.join();
}

void SipStack::startDUM()
{
  m_timer.expires_after(std::chrono::milliseconds(50));
  m_timer.async_wait([this](const boost::system::error_code& ec)
                     { processDUMOnTimer(); });
  m_asio_eventFd.async_wait(boost::asio::posix::stream_descriptor::wait_read,
                            [this](const boost::system::error_code& ec)
                            { processDUMOnEventFd(); });

  m_DUMThread = std::thread([this]() { m_IOContext.run(); });
}

void SipStack::processDUMOnEventFd()
{
  uint64_t u;
  if (::read(m_stackEventFd, &u, sizeof(u)) <= 0) {
    throw std::runtime_error("Something went wrong on reading event_fd");
  }

  while (m_DUM.process())
    ;
  m_asio_eventFd.async_wait(boost::asio::posix::stream_descriptor::wait_read,
                            [this](const boost::system::error_code& ec)
                            { processDUMOnEventFd(); });
}

void SipStack::processDUMOnTimer()
{
  while (m_DUM.process())
    ;
  m_timer.expires_after(std::chrono::milliseconds(50));
  m_timer.async_wait([this](const boost::system::error_code& ec)
                     { processDUMOnTimer(); });
}

void SipStack::notify(const Command& cmd)
{
  const auto& sip_cmd = dynamic_cast<const SignalCommand&>(cmd);
  std::cout << "Received SIP command: "
            << util::enum_traits<SignalCommandType>::get_name(
                 (SignalCommandType)sip_cmd.type())
            << std::endl;
  boost::asio::post(
    m_IOContext,
    [this]()
    {
      resip::NameAddr target("sip:200@127.0.0.1:5062");
      auto customProfile = std::make_shared<resip::UserProfile>();
      customProfile->setUserAgent("ORange PBX/1.0");
      customProfile->setDefaultFrom(resip::NameAddr("sip:100@127.0.0.1:5060"));
      auto inviteSession =
        m_DUM.makeInviteSession(target, customProfile, nullptr);
      m_DUM.send(inviteSession);
    });
}

void SipStack::init()
{
  CommandBus::instance().subscribe(util::enum_traits<SignalCommandType>::all,
                                   shared_from_this());
}
