#include <SipStack.hpp>

#include <resip/dum/InviteSessionHandler.hxx>
#include <resip/dum/DialogUsageManager.hxx>
#include <resip/stack/EventStackThread.hxx>
#include <resip/stack/SipStack.hxx>
#include <rutil/Logger.hxx>
#include <rutil/Log.hxx>

SipStack::SipStack()
    : m_masterProfile(std::make_shared<resip::MasterProfile>()),
      m_pollGrp(resip::FdPollGrp::create()),
      m_intr(new resip::EventThreadInterruptor(*m_pollGrp)),
      m_stack(0, resip::DnsStub::EmptyNameserverList, m_intr, false, 0, 0, m_pollGrp),
      m_DUM(m_stack),
      m_stackThread(m_stack, *m_intr, *m_pollGrp)
{
    resip::Log::initialize(resip::Log::Type::OnlyExternal, resip::Log::Debug, "MySipApp", m_logger);
    m_stack.addTransport(resip::UDP, 5060);

    m_masterProfile->addSupportedMethod(resip::INVITE);
    m_masterProfile->setUserAgent("ORange PBX/0.1");

    m_DUM.setMasterProfile(m_masterProfile);
    m_DUM.setInviteSessionHandler(&m_sessionHandler);

    m_stackThread.run();

    m_DUMThread = std::move(std::thread([this]() {
        while(m_running) {
            m_DUM.process(50);
        }
    }));
}

SipStack::~SipStack()
{
    m_running = false;
    if(m_DUMThread.joinable()) {
        m_DUMThread.join();
    }
    m_stackThread.shutdown();
    m_stackThread.join();
}

void SipStack::createAndInitInstance()
{
    s_instance = new SipStack();
}

SipStack* SipStack::getInstance()
{
    if(s_instance == nullptr) {
        throw std::runtime_error("SipStack instance is not created");
    }

    return s_instance;
}

SipStack* SipStack::s_instance = nullptr;