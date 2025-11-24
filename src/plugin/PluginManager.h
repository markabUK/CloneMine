#pragma once

#include <memory>
#include <vector>
#include <string>
#include <string_view>

namespace clonemine {

class LuaSandbox;

struct Plugin {
    std::string name;
    std::string version;
    std::unique_ptr<LuaSandbox> sandbox;
};

class PluginManager {
public:
    PluginManager();
    ~PluginManager() = default;

    // Delete copy operations
    PluginManager(const PluginManager&) = delete;
    PluginManager& operator=(const PluginManager&) = delete;

    bool loadPlugin(std::string_view filename);
    void loadPluginsFromDirectory(std::string_view directory);
    void initializePlugins();
    void callHook(std::string_view hookName);
    
    template<typename T>
    void callHook(std::string_view hookName, T arg);

private:
    std::vector<Plugin> m_plugins;
};

} // namespace clonemine
