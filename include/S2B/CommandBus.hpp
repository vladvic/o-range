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
#include <util/Singleton.hpp>
#include <vector>

#include "Command.hpp"

template<typename T>
concept CommandTypeContainer = requires(T t) {
  std::begin(t);
  std::end(t);
  {
    static_cast<CommandType>(*std::begin(t))
  } -> std::convertible_to<CommandType>;
};

class CommandBusSubscriber
{
public:
  virtual void notify(const Command& cmd) = 0;
};

class CommandBusInterface
{
public:
  virtual ~CommandBusInterface() = default;
  virtual void publish(std::unique_ptr<Command>&& cmd) = 0;
  virtual void subscribe(CommandType,
                         std::shared_ptr<CommandBusSubscriber>) = 0;

  template<typename T>
    requires std::is_convertible_v<
      CommandType,
      decltype(static_cast<CommandType>(std::declval<T>()))>
  void subscribe(T type, std::shared_ptr<CommandBusSubscriber> sub)
  {
    subscribe(static_cast<CommandType>(type), sub);
  }

  template<CommandTypeContainer T>
  void subscribe(const T& types, std::shared_ptr<CommandBusSubscriber> sub)
  {
    for (auto t : types) {
      subscribe(t, sub);
    }
  }
};

class CommandBus
  : public CommandBusInterface
  , public util::Singleton<CommandBusInterface>
{
public:
  virtual ~CommandBus() = default;
};

class MainCommandBus : public CommandBus
{
  boost::asio::thread_pool m_ioct;
  boost::asio::executor_work_guard<boost::asio::thread_pool::executor_type>
    m_workGuard;

  using SubscriberList = std::vector<std::weak_ptr<CommandBusSubscriber>>;

  std::map<CommandType, SubscriberList> m_subscribers;

  void notifyAll(const Command& cmd);

  template<typename T>
  void post(T&& callback)
  {
    boost::asio::post(m_ioct.executor(), std::move(callback));
  }

public:
  MainCommandBus();
  ~MainCommandBus();

  void subscribe(CommandType, std::shared_ptr<CommandBusSubscriber>) override;
  void publish(std::unique_ptr<Command>&& cmd) override;
};
