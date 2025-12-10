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
#include <util/Singleton.hpp>

#include "Command.hpp"

class CommandBusSubscriber
{
public:
  virtual void notify(const Command & cmd) = 0;
};

class CommandBusInterface
{
public:
  virtual ~CommandBusInterface() = default;
  virtual void publish(std::unique_ptr<Command> && cmd) = 0;
  virtual void subscribe(CommandType, std::shared_ptr<CommandBusSubscriber>) = 0;

  template<typename T>
  void subscribe(T type, std::shared_ptr<CommandBusSubscriber> sub)
  {
    subscribe((CommandType)(type), sub);
  }
};

class CommandBus
  : public CommandBusInterface
  , public util::Singleton<CommandBusInterface>
{
public:
  virtual ~CommandBus() = default;
};

class MainCommandBus
  : public CommandBus
{
  boost::asio::thread_pool m_ioct;
  boost::asio::executor_work_guard<boost::asio::thread_pool::executor_type> m_workGuard;

  using SubscriberList = std::vector<std::weak_ptr<CommandBusSubscriber>>;

  std::map<CommandType, SubscriberList> m_subscribers;

  void notifyAll(const Command & cmd);

  template<typename T>
  void post(T && callback)
  {
    boost::asio::post(m_ioct.executor(),
      std::move(callback)
    );
  }

public:
  MainCommandBus();
  ~MainCommandBus();

  void subscribe(CommandType, std::shared_ptr<CommandBusSubscriber>) override;
  void publish(std::unique_ptr<Command> && cmd) override;
};
