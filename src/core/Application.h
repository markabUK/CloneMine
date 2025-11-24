#pragma once

#include <memory>
#include <string_view>

namespace clonemine {

class Window;
class Renderer;
class World;
class PluginManager;

class Application {
public:
    Application(std::string_view title, uint32_t width, uint32_t height);
    ~Application();

    // Delete copy operations for memory safety
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;
    
    // Allow move operations
    Application(Application&&) noexcept = default;
    Application& operator=(Application&&) noexcept = default;

    void run();
    void shutdown();

    [[nodiscard]] bool isRunning() const noexcept { return m_running; }

private:
    void processInput();
    void update(float deltaTime);
    void render();

    std::unique_ptr<Window> m_window;
    std::unique_ptr<Renderer> m_renderer;
    std::unique_ptr<World> m_world;
    std::unique_ptr<PluginManager> m_pluginManager;
    bool m_running{true};
};

} // namespace clonemine
