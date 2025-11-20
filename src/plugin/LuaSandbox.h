#pragma once

#include <string>
#include <string_view>
#include <memory>

struct lua_State;

namespace clonemine {

class LuaSandbox {
public:
    LuaSandbox();
    ~LuaSandbox();

    // Delete copy operations
    LuaSandbox(const LuaSandbox&) = delete;
    LuaSandbox& operator=(const LuaSandbox&) = delete;

    bool loadScript(std::string_view filename);
    bool executeString(std::string_view code);
    
    void callFunction(std::string_view functionName);
    template<typename T>
    void callFunction(std::string_view functionName, T arg);
    
    [[nodiscard]] lua_State* getState() const noexcept { return m_state; }

private:
    void setupSandbox();
    void removeUnsafeFunctions();
    void registerSafeAPI();

    lua_State* m_state{nullptr};
};

} // namespace clonemine
