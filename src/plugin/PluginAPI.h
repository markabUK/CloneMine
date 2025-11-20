#pragma once

struct lua_State;

namespace clonemine {

class PluginAPI {
public:
    static void registerAPI(lua_State* L);

private:
    // Safe API functions that plugins can call
    static int lua_log(lua_State* L);
    static int lua_getBlockType(lua_State* L);
    static int lua_setBlockType(lua_State* L);
};

} // namespace clonemine
