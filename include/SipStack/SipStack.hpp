#pragma once

#include <ORangeSessionHandler.hpp>
#include <S2B/CommandBus.hpp>
#include <boost/asio/io_context.hpp>
#include <resip/dum/MasterProfile.hxx>
#include <resip/stack/EventStackThread.hxx>
#include <resip/stack/SipStack.hxx>
#include <util/Singleton.hpp>
#include <memory>
#include <thread>

class CustomLogger;

namespace resip {
class FdPollGrp;
}

class CommandBus;

class SipStack : public util::Singleton<SipStack>,
                 public CommandBusSubscriber,
                 public std::enable_shared_from_this<SipStack> {
 public:
  SipStack();
  ~SipStack();
  auto& getDUMIOContext() { return m_IOContext; }
  void notify(const Command& cmd) override;
  void subscribe(CommandBus& bus);

 private:
  void startDUM();
  std::unique_ptr<CustomLogger> m_logger;
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
