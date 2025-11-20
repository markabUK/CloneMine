#include "plugin/LuaSandbox.h"
#include "plugin/PluginAPI.h"
#include <lua.hpp>
#include <fstream>
#include <sstream>

namespace clonemine {

LuaSandbox::LuaSandbox() {
    m_state = luaL_newstate();
    if (m_state) {
        luaL_openlibs(m_state);
        setupSandbox();
    }
}

LuaSandbox::~LuaSandbox() {
    if (m_state) {
        lua_close(m_state);
    }
}

void LuaSandbox::setupSandbox() {
    removeUnsafeFunctions();
    registerSafeAPI();
}

void LuaSandbox::removeUnsafeFunctions() {
    // Remove dangerous functions to prevent system access
    lua_pushnil(m_state);
    lua_setglobal(m_state, "dofile");
    
    lua_pushnil(m_state);
    lua_setglobal(m_state, "loadfile");
    
    lua_pushnil(m_state);
    lua_setglobal(m_state, "load");
    
    // Remove os library functions that could be dangerous
    lua_getglobal(m_state, "os");
    if (lua_istable(m_state, -1)) {
        lua_pushnil(m_state);
        lua_setfield(m_state, -2, "execute");
        
        lua_pushnil(m_state);
        lua_setfield(m_state, -2, "exit");
        
        lua_pushnil(m_state);
        lua_setfield(m_state, -2, "remove");
        
        lua_pushnil(m_state);
        lua_setfield(m_state, -2, "rename");
        
        lua_pushnil(m_state);
        lua_setfield(m_state, -2, "tmpname");
    }
    lua_pop(m_state, 1);
    
    // Remove io library for file system access
    lua_pushnil(m_state);
    lua_setglobal(m_state, "io");
    
    // Remove debug library
    lua_pushnil(m_state);
    lua_setglobal(m_state, "debug");
    
    // Remove package library to prevent loading external modules
    lua_pushnil(m_state);
    lua_setglobal(m_state, "package");
}

void LuaSandbox::registerSafeAPI() {
    // Register safe CloneMine API
    PluginAPI::registerAPI(m_state);
}

bool LuaSandbox::loadScript(std::string_view filename) {
    std::ifstream file(filename.data());
    if (!file.is_open()) {
        return false;
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string code = buffer.str();
    
    return executeString(code);
}

bool LuaSandbox::executeString(std::string_view code) {
    if (luaL_dostring(m_state, code.data()) != LUA_OK) {
        // Error handling
        lua_pop(m_state, 1);
        return false;
    }
    return true;
}

void LuaSandbox::callFunction(std::string_view functionName) {
    lua_getglobal(m_state, functionName.data());
    if (lua_isfunction(m_state, -1)) {
        if (lua_pcall(m_state, 0, 0, 0) != LUA_OK) {
            lua_pop(m_state, 1);
        }
    } else {
        lua_pop(m_state, 1);
    }
}

template<typename T>
void LuaSandbox::callFunction(std::string_view functionName, T arg) {
    lua_getglobal(m_state, functionName.data());
    if (lua_isfunction(m_state, -1)) {
        if constexpr (std::is_same_v<T, float> || std::is_same_v<T, double>) {
            lua_pushnumber(m_state, arg);
        } else if constexpr (std::is_same_v<T, int>) {
            lua_pushinteger(m_state, arg);
        } else if constexpr (std::is_same_v<T, const char*> || std::is_same_v<T, std::string>) {
            lua_pushstring(m_state, arg);
        }
        
        if (lua_pcall(m_state, 1, 0, 0) != LUA_OK) {
            lua_pop(m_state, 1);
        }
    } else {
        lua_pop(m_state, 1);
    }
}

// Explicit template instantiations
template void LuaSandbox::callFunction<float>(std::string_view, float);
template void LuaSandbox::callFunction<double>(std::string_view, double);
template void LuaSandbox::callFunction<int>(std::string_view, int);

} // namespace clonemine
