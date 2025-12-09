#pragma once

#undef LOG_DEBUG
#undef LOG_INFO

#include <CustomLogger.hpp>
#include <ORangeSessionHandler.hpp>
#include <boost/asio/io_context.hpp>
#include <memory>
#include <resip/dum/MasterProfile.hxx>
#include <resip/stack/EventStackThread.hxx>
#include <resip/stack/SipStack.hxx>
#include <thread>
#include <util/Singleton.hpp>

namespace resip {
class FdPollGrp;
}

class SipStack : public util::Singleton<SipStack> {
 public:
  SipStack();
  ~SipStack();
  auto& getDUMIOContext() { return m_IOContext; }

 private:
  void startDUM();
  CustomLogger m_logger;
  std::shared_ptr<resip::MasterProfile> m_masterProfile;
  resip::FdPollGrp* m_pollGrp;
  resip::EventThreadInterruptor* m_intr;
  resip::SipStack m_stack;
  resip::DialogUsageManager m_DUM;
  resip::EventStackThread m_stackThread;
  std::thread m_DUMThread;
  boost::asio::io_context m_IOContext;
  ORangeSessionHandler m_sessionHandler;
  bool m_running{true};
};