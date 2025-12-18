#pragma once

#include <glm/glm.hpp>
#include <unordered_set>

struct GLFWwindow;

namespace clonemine {

// Key codes (subset of GLFW keys)
enum class Key {
    W = 87,
    A = 65,
    S = 83,
    D = 68,
    SPACE = 32,
    LEFT_SHIFT = 340,
    ESCAPE = 256,
    ENTER = 257,
    T = 84  // For chat
};

enum class MouseButton {
    LEFT = 0,
    RIGHT = 1,
    MIDDLE = 2
};

class InputManager {
public:
    explicit InputManager(GLFWwindow* window);
    ~InputManager() = default;
    
    // Update input state (call once per frame)
    void update();
    
    // Key state queries
    [[nodiscard]] bool isKeyPressed(Key key) const;
    [[nodiscard]] bool isKeyJustPressed(Key key) const;
    [[nodiscard]] bool isKeyJustReleased(Key key) const;
    
    // Mouse state queries
    [[nodiscard]] bool isMouseButtonPressed(MouseButton button) const;
    [[nodiscard]] glm::vec2 getMousePosition() const { return m_mousePosition; }
    [[nodiscard]] glm::vec2 getMouseDelta() const { return m_mouseDelta; }
    
    // Movement vector from WASD
    [[nodiscard]] glm::vec3 getMovementInput() const;
    
    // Chat mode
    void setChatMode(bool enabled) { m_chatMode = enabled; }
    [[nodiscard]] bool isChatMode() const { return m_chatMode; }
    
    // Get typed text (for chat)
    [[nodiscard]] const std::string& getTypedText() const { return m_typedText; }
    void clearTypedText() { m_typedText.clear(); }
    
    // Check if any input has occurred (for AFK detection)
    [[nodiscard]] bool hasAnyInput() const;
    
private:
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    static void charCallback(GLFWwindow* window, unsigned int codepoint);
    
    GLFWwindow* m_window;
    
    std::unordered_set<int> m_keysPressed;
    std::unordered_set<int> m_keysJustPressed;
    std::unordered_set<int> m_keysJustReleased;
    
    std::unordered_set<int> m_mouseButtonsPressed;
    
    glm::vec2 m_mousePosition{0.0f};
    glm::vec2 m_lastMousePosition{0.0f};
    glm::vec2 m_mouseDelta{0.0f};
    
    bool m_chatMode{false};
    std::string m_typedText;
    bool m_firstMouse{true};
};

} // namespace clonemine
