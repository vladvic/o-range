/***************************************************
 * main.cpp
 * Created on Sat, 22 Nov 2025 13:42:51 +0000 by vladimir
 *
 * $Author$
 * $Rev$
 * $Date$
 ***************************************************/

#include <iostream>
#include <memory>

#include <Logger/Logger.hpp>
#include <R-Range/ArenaLocator.hpp>
#include <R-Range/Call.hpp>
#include <R-Range/Device.hpp>
#include <R-Range/Leg.hpp>
#include <R-Range/Number.hpp>
#include <R-Range/Object.hpp>
#include <R-Range/ObjectArena.hpp>
#include <R-Range/Session.hpp>

using namespace Logger;

class SimpleLogger : public Logger::ILoggerImpl
{
public:
  void emit_message(LogLevel lvl,
                    SinkType sink,
                    std::string_view msg,
                    const Logger::SourceLocation& loc) override
  {
    std::cerr << msg << std::endl;
  }
};

class SimpleSessionId : public SessionId
{
  intptr_t m_id;

public:
  SimpleSessionId(intptr_t id)
    : m_id(id)
  {
  }
  intptr_t hash() const override { return m_id; }
};

int main()
{
  SimpleLogger logger;
  ArenaLocator arenaLocator;
  std::map<intptr_t, std::weak_ptr<Session>> sessions;
  auto addSession = [&sessions](auto s)
  { sessions.emplace(s->id().hash(), s); };
  {
    std::weak_ptr<Device> dev0w;
    LOG_INFO("Creating arena");
    auto arena = Object::make<ObjectArena>();
    {
      {
        auto dev0 = std::make_shared<Device>("0");
        LOG_INFO("Adding device");
        arena->add(dev0);
        auto session0 =
          Object::make<Session>(std::make_unique<SimpleSessionId>(0), dev0);
        addSession(session0);
        LOG_INFO("Adding session");
        arena->add(session0);
        dev0w = dev0->as<decltype(dev0w)>();
      }

      auto dev1 = std::make_shared<Device>("1");
      LOG_INFO("Adding device");
      arena->add(dev1);
      auto session1 =
        Object::make<Session>(std::make_unique<SimpleSessionId>(1), dev1);
      addSession(session1);
      LOG_INFO("Adding session");
      arena->add(session1);

      auto dev2 = std::make_shared<Device>("2");
      LOG_INFO("Adding device");
      arena->add(dev2);
      auto session2 =
        Object::make<Session>(std::make_unique<SimpleSessionId>(2), dev1);
      addSession(session2);
      LOG_INFO("Adding session");
      arena->add(session2);
      LOG_INFO("Removing device");
      arena->remove(dev0w.lock());
      LOG_INFO("Going out of dev1/2 scope");
    }
    LOG_INFO("Removing session0");
    ArenaLocator::locate(0)->remove(sessions.at(0).lock());
    ArenaLocator::locate(1)->remove(sessions.at(1).lock());
    ArenaLocator::locate(2)->remove(sessions.at(2).lock());
  }
}
