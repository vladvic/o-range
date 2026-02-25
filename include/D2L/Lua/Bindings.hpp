/***************************************************
 * Bindings.hpp
 * Created on Tue, 24 Feb 2026 21:28:55 +0000 by vladimir
 *
 * $Author$
 * $Rev$
 * $Date$
 ***************************************************/
#pragma once

#include <S2B/CommandBus.hpp>
#include <S2B/SignalCommand.hpp>

#include <R-Range/ObjectArena.hpp>

struct lua_State;

namespace D2L::Lua {

void register_types(lua_State* state);
void set_globals(lua_State* state,
                 ObjectArena& arena,
                 CommandBusInterface& bus);
void set_cmd(lua_State* state, SignalCommand& cmd);

} // namespace D2L::Lua
