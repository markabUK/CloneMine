#include "plugin/PluginManager.h"
#include "plugin/LuaSandbox.h"
#include <filesystem>
#include <iostream>

namespace clonemine {

PluginManager::PluginManager() = default;

bool PluginManager::loadPlugin(std::string_view filename) {
    Plugin plugin;
    plugin.sandbox = std::make_unique<LuaSandbox>();
    
    if (!plugin.sandbox->loadScript(filename)) {
        return false;
    }
    
    // Extract plugin name from filename
    std::filesystem::path path(filename);
    plugin.name = path.stem().string();
    plugin.version = "1.0.0";
    
    m_plugins.push_back(std::move(plugin));
    return true;
}

void PluginManager::loadPluginsFromDirectory(std::string_view directory) {
    if (!std::filesystem::exists(directory)) {
        return;
    }
    
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        if (entry.path().extension() == ".lua") {
            loadPlugin(entry.path().string());
        }
    }
}

void PluginManager::initializePlugins() {
    for (auto& plugin : m_plugins) {
        plugin.sandbox->callFunction("onLoad");
    }
}

void PluginManager::callHook(std::string_view hookName) {
    for (auto& plugin : m_plugins) {
        plugin.sandbox->callFunction(hookName);
    }
}

template<typename T>
void PluginManager::callHook(std::string_view hookName, T arg) {
    for (auto& plugin : m_plugins) {
        plugin.sandbox->callFunction(hookName, arg);
    }
}

// Explicit template instantiations
template void PluginManager::callHook<float>(std::string_view, float);
template void PluginManager::callHook<double>(std::string_view, double);
template void PluginManager::callHook<int>(std::string_view, int);

} // namespace clonemine
