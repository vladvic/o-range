/***************************************************
 * CommandBus.hpp
 * Created on Sat, 22 Nov 2025 17:08:41 +0000 by vladimir
 *
 * $Author$
 * $Rev$
 * $Date$
 ***************************************************/
#pragma once

#include <boost/asio.hpp>
#include <map>
#include <memory>
#include <thread>
#include <vector>

#include "Command.hpp"

class CommandBusSubscriber
{
public:
  virtual void notify(const Command & cmd) = 0;
};

class CommandBus
{
  boost::asio::thread_pool m_ioct;
#if BOOST_ASIO_VERSION < 103400
  boost::asio::thread_pool::executor_type::work m_workGuard;
#else
  boost::asio::executor_work_guard<boost::asio::thread_pool::executor_type> m_workGuard;
#endif

  using SubscriberList = std::vector<std::weak_ptr<CommandBusSubscriber>>;

  std::map<CommandType, SubscriberList> m_subscribers;

  void notifyAll(const Command & cmd);

  template<typename T>
  void post(T && callback)
  {
#if BOOST_ASIO_VERSION < 103400
    m_ioct.executor().post(
#else
    boost::asio::post(m_ioct.executor(),
#endif
      std::move(callback)
    );
  }

public:
  CommandBus();
  ~CommandBus();

  void subscribe(CommandType, std::shared_ptr<CommandBusSubscriber>);
  void publish(std::unique_ptr<Command> && cmd);
};
