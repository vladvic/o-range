/***************************************************
 * Bindings.cpp
 * Created on Tue, 24 Feb 2026 21:28:55 +0000 by vladimir
 *
 * $Author$
 * $Rev$
 * $Date$
 ***************************************************/

#include <D2L/Lua/Bindings.hpp>

#include <lua.hpp>

#include <R-Range/Call.hpp>
#include <R-Range/Device.hpp>
#include <R-Range/Leg.hpp>
#include <R-Range/Number.hpp>
#include <R-Range/Session.hpp>

#include <cstdint>
#include <exception>

namespace {

constexpr const char* kArenaMT = "D2L.Arena";
constexpr const char* kDeviceMT = "D2L.Device";
constexpr const char* kNumberMT = "D2L.Number";
constexpr const char* kSessionMT = "D2L.Session";
constexpr const char* kLegMT = "D2L.Leg";
constexpr const char* kCallMT = "D2L.Call";
constexpr const char* kBusMT = "D2L.CommandBus";
constexpr const char* kCmdMT = "D2L.SignalCommand";

struct LuaArena
{
  ObjectArena* arena;
};

struct LuaBus
{
  CommandBusInterface* bus;
};

struct LuaCmd
{
  SignalCommand* cmd{ nullptr };
  std::unique_ptr<SignalCommand> owned;
};

template<typename T>
struct LuaShared
{
  std::shared_ptr<T> ptr;
};

template<typename T>
LuaShared<T>* checkShared(lua_State* L, int idx, const char* mt)
{
  return static_cast<LuaShared<T>*>(luaL_checkudata(L, idx, mt));
}

LuaArena* checkArena(lua_State* L, int idx)
{
  return static_cast<LuaArena*>(luaL_checkudata(L, idx, kArenaMT));
}

LuaBus* checkBus(lua_State* L, int idx)
{
  return static_cast<LuaBus*>(luaL_checkudata(L, idx, kBusMT));
}

LuaCmd* checkCmd(lua_State* L, int idx)
{
  return static_cast<LuaCmd*>(luaL_checkudata(L, idx, kCmdMT));
}

SignalCommand* require_cmd(lua_State* L, int idx)
{
  auto* ud = checkCmd(L, idx);
  if (!ud || !ud->cmd) {
    luaL_error(L, "SignalCommand is not available");
    return nullptr;
  }
  return ud->cmd;
}

template<typename T>
int gcShared(lua_State* L)
{
  auto* ud = static_cast<LuaShared<T>*>(lua_touserdata(L, 1));
  if (ud) {
    ud->~LuaShared<T>();
  }
  return 0;
}

template<typename T>
int pushShared(lua_State* L, std::shared_ptr<T> ptr, const char* mt)
{
  auto* ud =
    static_cast<LuaShared<T>*>(lua_newuserdata(L, sizeof(LuaShared<T>)));
  new (ud) LuaShared<T>{ std::move(ptr) };
  luaL_getmetatable(L, mt);
  lua_setmetatable(L, -2);
  return 1;
}

int pushArena(lua_State* L, ObjectArena* arena)
{
  auto* ud = static_cast<LuaArena*>(lua_newuserdata(L, sizeof(LuaArena)));
  ud->arena = arena;
  luaL_getmetatable(L, kArenaMT);
  lua_setmetatable(L, -2);
  return 1;
}

int pushBus(lua_State* L, CommandBusInterface* bus)
{
  auto* ud = static_cast<LuaBus*>(lua_newuserdata(L, sizeof(LuaBus)));
  ud->bus = bus;
  luaL_getmetatable(L, kBusMT);
  lua_setmetatable(L, -2);
  return 1;
}

int pushCmd(lua_State* L, SignalCommand* cmd)
{
  auto* ud = static_cast<LuaCmd*>(lua_newuserdata(L, sizeof(LuaCmd)));
  new (ud) LuaCmd{};
  ud->cmd = cmd;
  luaL_getmetatable(L, kCmdMT);
  lua_setmetatable(L, -2);
  return 1;
}

int arena_get_sessions(lua_State* L)
{
  auto* arena = checkArena(L, 1)->arena;
  auto sessions = arena->get<Session>();
  lua_newtable(L);
  int idx = 1;
  for (auto& s : sessions) {
    pushShared(L, s, kSessionMT);
    lua_rawseti(L, -2, idx++);
  }
  return 1;
}

int arena_get_devices(lua_State* L)
{
  auto* arena = checkArena(L, 1)->arena;
  auto devices = arena->get<Device>();
  lua_newtable(L);
  int idx = 1;
  for (auto& d : devices) {
    pushShared(L, d, kDeviceMT);
    lua_rawseti(L, -2, idx++);
  }
  return 1;
}

int arena_get_numbers(lua_State* L)
{
  auto* arena = checkArena(L, 1)->arena;
  auto numbers = arena->get<Number>();
  lua_newtable(L);
  int idx = 1;
  for (auto& n : numbers) {
    pushShared(L, n, kNumberMT);
    lua_rawseti(L, -2, idx++);
  }
  return 1;
}

int arena_get_legs(lua_State* L)
{
  auto* arena = checkArena(L, 1)->arena;
  auto legs = arena->get<Leg>();
  lua_newtable(L);
  int idx = 1;
  for (auto& leg : legs) {
    pushShared(L, leg, kLegMT);
    lua_rawseti(L, -2, idx++);
  }
  return 1;
}

int arena_get_calls(lua_State* L)
{
  auto* arena = checkArena(L, 1)->arena;
  auto calls = arena->get<Call>();
  lua_newtable(L);
  int idx = 1;
  for (auto& call : calls) {
    pushShared(L, call, kCallMT);
    lua_rawseti(L, -2, idx++);
  }
  return 1;
}

int arena_session(lua_State* L)
{
  auto* arena = checkArena(L, 1)->arena;
  auto id = static_cast<std::intptr_t>(luaL_checkinteger(L, 2));
  try {
    auto session = arena->session(id);
    if (!session) {
      lua_pushnil(L);
      return 1;
    }
    return pushShared(L, session, kSessionMT);
  }
  catch (const std::exception&) {
    lua_pushnil(L);
    return 1;
  }
}

int arena_add(lua_State* L)
{
  auto* arena = checkArena(L, 1)->arena;

  if (auto* dev =
        static_cast<LuaShared<Device>*>(luaL_testudata(L, 2, kDeviceMT)))
  {
    arena->add(dev->ptr);
    return 0;
  }
  if (auto* num =
        static_cast<LuaShared<Number>*>(luaL_testudata(L, 2, kNumberMT)))
  {
    arena->add(num->ptr);
    return 0;
  }
  if (auto* sess =
        static_cast<LuaShared<Session>*>(luaL_testudata(L, 2, kSessionMT)))
  {
    arena->add(sess->ptr);
    return 0;
  }
  if (auto* leg = static_cast<LuaShared<Leg>*>(luaL_testudata(L, 2, kLegMT))) {
    arena->add(leg->ptr);
    return 0;
  }
  if (auto* call = static_cast<LuaShared<Call>*>(luaL_testudata(L, 2, kCallMT)))
  {
    arena->add(call->ptr);
    return 0;
  }

  return luaL_error(L, "Unsupported type for arena:add");
}

int arena_remove(lua_State* L)
{
  auto* arena = checkArena(L, 1)->arena;

  if (auto* dev =
        static_cast<LuaShared<Device>*>(luaL_testudata(L, 2, kDeviceMT)))
  {
    arena->remove(dev->ptr);
    return 0;
  }
  if (auto* num =
        static_cast<LuaShared<Number>*>(luaL_testudata(L, 2, kNumberMT)))
  {
    arena->remove(num->ptr);
    return 0;
  }
  if (auto* sess =
        static_cast<LuaShared<Session>*>(luaL_testudata(L, 2, kSessionMT)))
  {
    arena->remove(sess->ptr);
    return 0;
  }
  if (auto* leg = static_cast<LuaShared<Leg>*>(luaL_testudata(L, 2, kLegMT))) {
    arena->remove(leg->ptr);
    return 0;
  }
  if (auto* call = static_cast<LuaShared<Call>*>(luaL_testudata(L, 2, kCallMT)))
  {
    arena->remove(call->ptr);
    return 0;
  }

  return luaL_error(L, "Unsupported type for arena:remove");
}

int device_id(lua_State* L)
{
  return luaL_error(L, "Device id getter not available");
}

int device_set_id(lua_State* L)
{
  return luaL_error(L, "Device id setter not available");
}

int device_number(lua_State* L)
{
  auto dev = checkShared<Device>(L, 1, kDeviceMT)->ptr;
  auto num = dev->number();
  if (!num) {
    lua_pushnil(L);
    return 1;
  }
  return pushShared(L, num, kNumberMT);
}

int device_set_number(lua_State* L)
{
  return luaL_error(L, "Device number setter not available");
}

int number_value(lua_State* L)
{
  auto num = checkShared<Number>(L, 1, kNumberMT)->ptr;
  lua_pushstring(L, num->number().c_str());
  return 1;
}

int number_set_value(lua_State* L)
{
  return luaL_error(L, "Number setter not available");
}

int session_id_hash(lua_State* L)
{
  auto session = checkShared<Session>(L, 1, kSessionMT)->ptr;
  lua_pushinteger(L, static_cast<lua_Integer>(session->id().hash()));
  return 1;
}

int session_device(lua_State* L)
{
  auto session = checkShared<Session>(L, 1, kSessionMT)->ptr;
  auto dev = session->device();
  if (!dev) {
    lua_pushnil(L);
    return 1;
  }
  return pushShared(L, dev, kDeviceMT);
}

int session_set_device(lua_State* L)
{
  return luaL_error(L, "Session device setter not available");
}

int session_leg(lua_State* L)
{
  auto session = checkShared<Session>(L, 1, kSessionMT)->ptr;
  auto leg = session->leg();
  if (!leg) {
    lua_pushnil(L);
    return 1;
  }
  return pushShared(L, leg, kLegMT);
}

int session_set_leg(lua_State* L)
{
  auto session = checkShared<Session>(L, 1, kSessionMT)->ptr;
  auto leg = checkShared<Leg>(L, 2, kLegMT)->ptr;
  session->setLeg(leg);
  return 0;
}

int leg_session(lua_State* L)
{
  auto leg = checkShared<Leg>(L, 1, kLegMT)->ptr;
  auto session = leg->session();
  if (!session) {
    lua_pushnil(L);
    return 1;
  }
  return pushShared(L, session, kSessionMT);
}

int leg_set_session(lua_State* L)
{
  auto leg = checkShared<Leg>(L, 1, kLegMT)->ptr;
  auto session = checkShared<Session>(L, 2, kSessionMT)->ptr;
  leg->setSession(session);
  return 0;
}

int leg_call(lua_State* L)
{
  auto leg = checkShared<Leg>(L, 1, kLegMT)->ptr;
  auto call = leg->call();
  if (!call) {
    lua_pushnil(L);
    return 1;
  }
  return pushShared(L, call, kCallMT);
}

int call_leg(lua_State* L)
{
  auto call = checkShared<Call>(L, 1, kCallMT)->ptr;
  auto leg = call->leg();
  if (!leg) {
    lua_pushnil(L);
    return 1;
  }
  return pushShared(L, leg, kLegMT);
}

int call_set_initiating_leg(lua_State* L)
{
  auto call = checkShared<Call>(L, 1, kCallMT)->ptr;
  auto leg = checkShared<Leg>(L, 2, kLegMT)->ptr;
  call->setInitiatingLeg(leg);
  return 0;
}

int call_set_outbound_leg(lua_State* L)
{
  auto call = checkShared<Call>(L, 1, kCallMT)->ptr;
  auto leg = checkShared<Leg>(L, 2, kLegMT)->ptr;
  call->setOutboundLeg(leg);
  return 0;
}

int bus_publish(lua_State* L)
{
  auto* bus = checkBus(L, 1)->bus;
  auto* ud = checkCmd(L, 2);
  auto* cmd = require_cmd(L, 2);
  if (!cmd) {
    return 0;
  }

  if (ud->owned) {
    bus->publish(std::move(ud->owned));
    ud->cmd = nullptr;
  }
  else {
    std::unique_ptr<SignalCommand> out;
    auto type = cmd->type();
    if (cmd->hasEnumType<SignalCommandType>()) {
      out =
        std::make_unique<SignalCommand>(static_cast<SignalCommandType>(type));
    }
    else {
      out = std::make_unique<SignalCommand>(static_cast<SignalEventType>(type));
    }

    out->setCompletionToken(cmd->getCompletionToken());
    out->source() = cmd->source();
    out->destination() = cmd->destination();
    out->rejectReason() = cmd->rejectReason();

    bus->publish(std::move(out));
  }
  return 0;
}

int cmd_type(lua_State* L)
{
  auto* cmd = require_cmd(L, 1);
  if (!cmd) {
    return 0;
  }
  lua_pushinteger(L, static_cast<lua_Integer>(cmd->type()));
  return 1;
}

int cmd_set_type(lua_State* L)
{
  return luaL_error(L, "SignalCommand type setter not available");
}

int cmd_source(lua_State* L)
{
  auto* cmd = require_cmd(L, 1);
  if (!cmd) {
    return 0;
  }
  lua_pushstring(L, cmd->source().c_str());
  return 1;
}

int cmd_set_source(lua_State* L)
{
  auto* cmd = require_cmd(L, 1);
  if (!cmd) {
    return 0;
  }
  const char* s = luaL_checkstring(L, 2);
  cmd->source() = s;
  return 0;
}

int cmd_destination(lua_State* L)
{
  auto* cmd = require_cmd(L, 1);
  if (!cmd) {
    return 0;
  }
  lua_pushstring(L, cmd->destination().c_str());
  return 1;
}

int cmd_set_destination(lua_State* L)
{
  auto* cmd = require_cmd(L, 1);
  if (!cmd) {
    return 0;
  }
  const char* s = luaL_checkstring(L, 2);
  cmd->destination() = s;
  return 0;
}

int cmd_reject_reason(lua_State* L)
{
  auto* cmd = require_cmd(L, 1);
  if (!cmd) {
    return 0;
  }
  lua_pushinteger(L, static_cast<lua_Integer>(cmd->rejectReason()));
  return 1;
}

int cmd_set_reject_reason(lua_State* L)
{
  auto* cmd = require_cmd(L, 1);
  if (!cmd) {
    return 0;
  }
  auto reason = static_cast<SignalCommandRejectReason>(luaL_checkinteger(L, 2));
  cmd->rejectReason() = reason;
  return 0;
}

int cmd_gc(lua_State* L)
{
  auto* ud = static_cast<LuaCmd*>(lua_touserdata(L, 1));
  if (ud) {
    ud->owned.reset();
    ud->cmd = nullptr;
  }
  return 0;
}

int cmd_new(lua_State* L)
{
  auto type = static_cast<CommandType>(luaL_checkinteger(L, 1));
  std::unique_ptr<SignalCommand> cmd;

  if (type >= static_cast<CommandType>(SignalEventType::CREATED) &&
      type <= static_cast<CommandType>(SignalEventType::INVALID))
  {
    cmd = std::make_unique<SignalCommand>(static_cast<SignalEventType>(type));
  }
  else {
    cmd = std::make_unique<SignalCommand>(static_cast<SignalCommandType>(type));
  }

  auto* ud = static_cast<LuaCmd*>(lua_newuserdata(L, sizeof(LuaCmd)));
  new (ud) LuaCmd{};
  ud->cmd = cmd.get();
  ud->owned = std::move(cmd);
  luaL_getmetatable(L, kCmdMT);
  lua_setmetatable(L, -2);
  return 1;
}

void register_arena(lua_State* L)
{
  luaL_newmetatable(L, kArenaMT);
  lua_newtable(L);
  luaL_Reg methods[] = {
    { "get_sessions", arena_get_sessions },
    { "get_devices", arena_get_devices },
    { "get_numbers", arena_get_numbers },
    { "get_legs", arena_get_legs },
    { "get_calls", arena_get_calls },
    { "session", arena_session },
    { "add", arena_add },
    { "remove", arena_remove },
    { nullptr, nullptr },
  };
  luaL_setfuncs(L, methods, 0);
  lua_setfield(L, -2, "__index");
  lua_pop(L, 1);
}

template<typename T>
void register_shared_mt(lua_State* L, const char* mt, const luaL_Reg* methods)
{
  luaL_newmetatable(L, mt);
  lua_newtable(L);
  luaL_setfuncs(L, methods, 0);
  lua_setfield(L, -2, "__index");

  lua_pushcfunction(L, gcShared<T>);
  lua_setfield(L, -2, "__gc");
  lua_pop(L, 1);
}

void register_device(lua_State* L)
{
  luaL_Reg methods[] = {
    { "id", device_id },         { "set_id", device_set_id },
    { "number", device_number }, { "set_number", device_set_number },
    { nullptr, nullptr },
  };
  register_shared_mt<Device>(L, kDeviceMT, methods);
}

void register_number(lua_State* L)
{
  luaL_Reg methods[] = {
    { "value", number_value },
    { "set_value", number_set_value },
    { nullptr, nullptr },
  };
  register_shared_mt<Number>(L, kNumberMT, methods);
}

void register_session(lua_State* L)
{
  luaL_Reg methods[] = {
    { "id_hash", session_id_hash },       { "device", session_device },
    { "set_device", session_set_device }, { "leg", session_leg },
    { "set_leg", session_set_leg },       { nullptr, nullptr },
  };
  register_shared_mt<Session>(L, kSessionMT, methods);
}

void register_leg(lua_State* L)
{
  luaL_Reg methods[] = {
    { "session", leg_session },
    { "set_session", leg_set_session },
    { "call", leg_call },
    { nullptr, nullptr },
  };
  register_shared_mt<Leg>(L, kLegMT, methods);
}

void register_call(lua_State* L)
{
  luaL_Reg methods[] = {
    { "leg", call_leg },
    { "set_initiating_leg", call_set_initiating_leg },
    { "set_outbound_leg", call_set_outbound_leg },
    { nullptr, nullptr },
  };
  register_shared_mt<Call>(L, kCallMT, methods);
}

void register_bus(lua_State* L)
{
  luaL_newmetatable(L, kBusMT);
  lua_newtable(L);
  luaL_Reg methods[] = {
    { "publish", bus_publish },
    { nullptr, nullptr },
  };
  luaL_setfuncs(L, methods, 0);
  lua_setfield(L, -2, "__index");
  lua_pop(L, 1);
}

void register_cmd(lua_State* L)
{
  luaL_newmetatable(L, kCmdMT);
  lua_newtable(L);
  luaL_Reg methods[] = {
    { "type", cmd_type },
    { "set_type", cmd_set_type },
    { "source", cmd_source },
    { "set_source", cmd_set_source },
    { "destination", cmd_destination },
    { "set_destination", cmd_set_destination },
    { "reject_reason", cmd_reject_reason },
    { "set_reject_reason", cmd_set_reject_reason },
    { nullptr, nullptr },
  };
  luaL_setfuncs(L, methods, 0);
  lua_setfield(L, -2, "__index");

  lua_pushcfunction(L, cmd_gc);
  lua_setfield(L, -2, "__gc");
  lua_pop(L, 1);

  lua_newtable(L);
  lua_pushcfunction(L, cmd_new);
  lua_setfield(L, -2, "new");
  lua_setglobal(L, "SignalCommand");
}

void register_all(lua_State* L)
{
  register_arena(L);
  register_device(L);
  register_number(L);
  register_session(L);
  register_leg(L);
  register_call(L);
  register_bus(L);
  register_cmd(L);
}

} // namespace

namespace D2L::Lua {

void register_types(lua_State* state)
{
  register_all(state);
}

void set_globals(lua_State* state, ObjectArena& arena, CommandBusInterface& bus)
{
  pushArena(state, &arena);
  lua_setglobal(state, "arena");
  pushBus(state, &bus);
  lua_setglobal(state, "bus");
}

void set_cmd(lua_State* state, SignalCommand& cmd)
{
  pushCmd(state, &cmd);
  lua_setglobal(state, "cmd");
}

} // namespace D2L::Lua
