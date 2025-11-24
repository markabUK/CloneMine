#include "plugin/PluginAPI.h"
#include <lua.hpp>
#include <iostream>

namespace clonemine {

void PluginAPI::registerAPI(lua_State* L) {
    // Register log function
    lua_pushcfunction(L, lua_log);
    lua_setglobal(L, "log");
    
    // Register block manipulation functions
    lua_pushcfunction(L, lua_getBlockType);
    lua_setglobal(L, "getBlockType");
    
    lua_pushcfunction(L, lua_setBlockType);
    lua_setglobal(L, "setBlockType");
}

int PluginAPI::lua_log(lua_State* L) {
    const char* message = luaL_checkstring(L, 1);
    std::cout << "[Plugin] " << message << std::endl;
    return 0;
}

int PluginAPI::lua_getBlockType(lua_State* L) {
    // int x = luaL_checkinteger(L, 1);
    // int y = luaL_checkinteger(L, 2);
    // int z = luaL_checkinteger(L, 3);
    
    // Placeholder - would query the actual world
    lua_pushinteger(L, 0);
    return 1;
}

int PluginAPI::lua_setBlockType(lua_State* L) {
    // int x = luaL_checkinteger(L, 1);
    // int y = luaL_checkinteger(L, 2);
    // int z = luaL_checkinteger(L, 3);
    // int blockType = luaL_checkinteger(L, 4);
    
    // Placeholder - would modify the actual world
    return 0;
}

} // namespace clonemine
