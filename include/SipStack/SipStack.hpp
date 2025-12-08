#pragma once

#include <memory>
#include <thread>

#include <resip/stack/SipStack.hxx>
#include <resip/stack/EventStackThread.hxx>
#include <resip/dum/MasterProfile.hxx>

#include <ORangeSessionHandler.hpp>
#include <CustomLogger.hpp>

namespace resip {
    class FdPollGrp;
}

class SipStack {
public:
    SipStack();
    ~SipStack();
    static void createAndInitInstance();
    static SipStack* getInstance();
private:
    static SipStack* s_instance;

    std::shared_ptr<resip::MasterProfile> m_masterProfile;
    resip::FdPollGrp *m_pollGrp;
    resip::EventThreadInterruptor* m_intr;
    resip::SipStack m_stack;
    resip::DialogUsageManager m_DUM;
    resip::EventStackThread m_stackThread;
    std::thread m_DUMThread;
    ORangeSessionHandler m_sessionHandler;
    CustomLogger m_logger;
    bool m_running{true};
};