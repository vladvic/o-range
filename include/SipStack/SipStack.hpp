#pragma once

extern "C" {
#include <unistd.h>
}

#include <ORangeSessionHandler.hpp>
#include <S2B/CommandBus.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/posix/stream_descriptor.hpp>
#include <boost/asio/steady_timer.hpp>
#include <memory>
#include <resip/dum/MasterProfile.hxx>
#include <resip/stack/EventStackThread.hxx>
#include <resip/stack/SipStack.hxx>
#include <thread>
#include <util/Singleton.hpp>

class CustomLogger;

namespace resip {
class FdPollGrp;
}

class CommandBus;

class CustomEventStackThread : public resip::EventStackThread {
 public:
  CustomEventStackThread(resip::SipStack& stack,
                         resip::EventThreadInterruptor& si,
                         resip::FdPollGrp& pollGrp, int eventFd)
      : resip::EventStackThread(stack, si, pollGrp), m_eventFd(eventFd) {}

 protected:
  virtual void afterProcess() override {
    uint64_t u = 1;
    ::write(m_eventFd, &u, sizeof(u));
  }

 private:
  int m_eventFd;
};

class SipStack : public util::Singleton<SipStack>,
                 public CommandBusSubscriber,
                 public std::enable_shared_from_this<SipStack> {
 public:
  SipStack();
  ~SipStack();
  auto& getDUMIOContext() { return m_IOContext; }
  void notify(const Command& cmd) override;
  void subscribe();

 private:
  void startDUM();
  void processDUMOnTimer();
  void processDUMOnEventFd();
  std::unique_ptr<CustomLogger> m_logger;
  std::shared_ptr<resip::MasterProfile> m_masterProfile;
  resip::FdPollGrp* m_pollGrp;
  resip::EventThreadInterruptor* m_intr;
  resip::SipStack m_stack;
  resip::DialogUsageManager m_DUM;
  int m_stackEventFd;
  CustomEventStackThread m_stackThread;
  std::thread m_DUMThread;
  boost::asio::io_context m_IOContext;
  boost::asio::posix::stream_descriptor m_asio_eventFd;
  boost::asio::steady_timer m_timer;
  ORangeSessionHandler m_sessionHandler;
  bool m_running{true};
};
