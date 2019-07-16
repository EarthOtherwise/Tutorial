#pragma once

extern "C"
{
	#include "Lua/lua.h"
	#include "Lua/lauxlib.h"
	#include "Lua/lualib.h"
}
#include "ErrHandler.h"

namespace Otherwise
{

struct LuaState
{
	lua_State *state;
};

void luaInit(lua_State *state);
void luaTerminate(lua_State *state);
bool isLuaOK(lua_State *state, int luaFunction);

}

