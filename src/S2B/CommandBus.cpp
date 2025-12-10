/***************************************************
 * MainCommandBus.cpp
 * Created on Sat, 22 Nov 2025 17:08:57 +0000 by vladimir
 *
 * $Author$
 * $Rev$
 * $Date$
 ***************************************************/

#include "CommandBus.hpp"
#include <Logger/Logger.hpp>

MainCommandBus::MainCommandBus()
  : m_ioct(1)
#if BOOST_ASIO_VERSION < 103400
  , m_workGuard(m_ioct)
#else
  , m_workGuard(boost::asio::make_work_guard(m_ioct))
#endif
{
}

MainCommandBus::~MainCommandBus()
{
  post( // Make sure all previous work is done before stop() is executed
    [this]() {
      m_ioct.stop();
    }
  );
  m_ioct.wait();
}

void MainCommandBus::notifyAll(const Command & cmd)
{
  auto subscribers = m_subscribers.find(cmd.type());
  if (subscribers != m_subscribers.end())
  {
    for (auto &sub : subscribers->second)
    {
      if (auto subscriber = sub.lock())
      {
        subscriber->notify(cmd);
      }
    }
  }
}

void MainCommandBus::subscribe(CommandType c, std::shared_ptr<CommandBusSubscriber> sub)
{
  post(
    [this, c, sub]() {
      m_subscribers[c].push_back(sub);
    }
  );
}

void MainCommandBus::publish(std::unique_ptr<Command> && cmd)
{
  auto callback = 
    [this, cmd = std::move(cmd)]() mutable {
      notifyAll(*cmd);
    };
  post(std::move(callback));
}
