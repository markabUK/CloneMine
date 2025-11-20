#pragma once

#include <cstdint>
#include <string_view>
#include <memory>

struct GLFWwindow;

namespace clonemine {

class Window {
public:
    Window(std::string_view title, uint32_t width, uint32_t height);
    ~Window();

    // Delete copy operations
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    
    // Allow move operations
    Window(Window&&) noexcept;
    Window& operator=(Window&&) noexcept;

    [[nodiscard]] bool shouldClose() const;
    void pollEvents();
    
    [[nodiscard]] uint32_t getWidth() const noexcept { return m_width; }
    [[nodiscard]] uint32_t getHeight() const noexcept { return m_height; }
    [[nodiscard]] GLFWwindow* getNativeWindow() const noexcept { return m_window; }
    
    [[nodiscard]] bool wasResized() const noexcept { return m_framebufferResized; }
    void resetResizedFlag() noexcept { m_framebufferResized = false; }

private:
    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

    GLFWwindow* m_window{nullptr};
    uint32_t m_width;
    uint32_t m_height;
    bool m_framebufferResized{false};
};

} // namespace clonemine
