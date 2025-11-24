#include "core/Application.h"
#include "core/Window.h"
#include "rendering/Renderer.h"
#include "world/World.h"
#include "plugin/PluginManager.h"
#include <chrono>
#include <stdexcept>

namespace clonemine {

Application::Application(std::string_view title, uint32_t width, uint32_t height)
    : m_window(std::make_unique<Window>(title, width, height))
    , m_renderer(std::make_unique<Renderer>(*m_window))
    , m_world(std::make_unique<World>())
    , m_pluginManager(std::make_unique<PluginManager>())
{
    // Load plugins from the plugins directory
    m_pluginManager->loadPluginsFromDirectory("plugins");
    m_pluginManager->initializePlugins();
}

Application::~Application() = default;

void Application::run() {
    using clock = std::chrono::high_resolution_clock;
    auto lastTime = clock::now();

    while (m_running && !m_window->shouldClose()) {
        auto currentTime = clock::now();
        auto deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;

        processInput();
        update(deltaTime);
        render();
        
        m_window->pollEvents();
    }

    m_renderer->waitIdle();
}

void Application::shutdown() {
    m_running = false;
}

void Application::processInput() {
    // Input processing will be handled here
    // Plugin callbacks for input
    m_pluginManager->callHook("onInput");
}

void Application::update(float deltaTime) {
    // Update world
    m_world->update(deltaTime);
    
    // Plugin update callbacks
    m_pluginManager->callHook("onUpdate", deltaTime);
}

void Application::render() {
    if (m_window->wasResized()) {
        m_renderer->handleResize();
        m_window->resetResizedFlag();
    }

    m_renderer->beginFrame();
    m_renderer->renderWorld(*m_world);
    m_renderer->endFrame();
    
    // Plugin render callbacks
    m_pluginManager->callHook("onRender");
}

} // namespace clonemine
