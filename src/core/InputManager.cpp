#include "InputManager.h"
#include <GLFW/glfw3.h>
#include <algorithm>

namespace clonemine {

InputManager::InputManager(GLFWwindow* window)
    : m_window(window)
{
    // Set user pointer for callbacks
    glfwSetWindowUserPointer(window, this);
    
    // Register callbacks
    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetCharCallback(window, charCallback);
    
    // Disable cursor for FPS-style camera
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void InputManager::update() {
    // Clear just pressed/released states
    m_keysJustPressed.clear();
    m_keysJustReleased.clear();
    
    // Calculate mouse delta
    m_mouseDelta = m_mousePosition - m_lastMousePosition;
    m_lastMousePosition = m_mousePosition;
    
    if (m_firstMouse) {
        m_mouseDelta = glm::vec2(0.0f);
        m_firstMouse = false;
    }
}

bool InputManager::isKeyPressed(Key key) const {
    return m_keysPressed.count(static_cast<int>(key)) > 0;
}

bool InputManager::isKeyJustPressed(Key key) const {
    return m_keysJustPressed.count(static_cast<int>(key)) > 0;
}

bool InputManager::isKeyJustReleased(Key key) const {
    return m_keysJustReleased.count(static_cast<int>(key)) > 0;
}

bool InputManager::isMouseButtonPressed(MouseButton button) const {
    return m_mouseButtonsPressed.count(static_cast<int>(button)) > 0;
}

glm::vec3 InputManager::getMovementInput() const {
    glm::vec3 movement(0.0f);
    
    if (!m_chatMode) {
        if (isKeyPressed(Key::W)) movement.z += 1.0f;
        if (isKeyPressed(Key::S)) movement.z -= 1.0f;
        if (isKeyPressed(Key::A)) movement.x -= 1.0f;
        if (isKeyPressed(Key::D)) movement.x += 1.0f;
        
        // Normalize diagonal movement
        if (glm::length(movement) > 0.0f) {
            movement = glm::normalize(movement);
        }
    }
    
    return movement;
}

void InputManager::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    (void)scancode;
    (void)mods;
    
    auto* manager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
    
    if (action == GLFW_PRESS) {
        manager->m_keysPressed.insert(key);
        manager->m_keysJustPressed.insert(key);
        
        // Toggle chat mode with T key
        if (key == static_cast<int>(Key::T) && !manager->m_chatMode) {
            manager->m_chatMode = true;
            manager->m_typedText.clear();
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        // Send chat or cancel with Enter/Escape
        else if (manager->m_chatMode) {
            if (key == static_cast<int>(Key::ENTER)) {
                // Chat message will be sent by client application
                manager->m_chatMode = false;
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
            else if (key == static_cast<int>(Key::ESCAPE)) {
                manager->m_chatMode = false;
                manager->m_typedText.clear();
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
        }
    }
    else if (action == GLFW_RELEASE) {
        manager->m_keysPressed.erase(key);
        manager->m_keysJustReleased.insert(key);
    }
}

void InputManager::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    (void)mods;
    
    auto* manager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
    
    if (action == GLFW_PRESS) {
        manager->m_mouseButtonsPressed.insert(button);
    }
    else if (action == GLFW_RELEASE) {
        manager->m_mouseButtonsPressed.erase(button);
    }
}

void InputManager::cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    auto* manager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
    manager->m_mousePosition = glm::vec2(static_cast<float>(xpos), static_cast<float>(ypos));
}

void InputManager::charCallback(GLFWwindow* window, unsigned int codepoint) {
    auto* manager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
    
    if (manager->m_chatMode && codepoint < 128) {
        // Only add printable ASCII characters
        if (codepoint == 8) { // Backspace
            if (!manager->m_typedText.empty()) {
                manager->m_typedText.pop_back();
            }
        }
        else if (codepoint >= 32 && manager->m_typedText.length() < 256) {
            manager->m_typedText += static_cast<char>(codepoint);
        }
    }
}

bool InputManager::hasAnyInput() const {
    // Check if any keys were just pressed
    if (!m_keysJustPressed.empty()) {
        return true;
    }
    
    // Check if mouse moved (ignore very small movements)
    if (glm::length(m_mouseDelta) > 0.1f) {
        return true;
    }
    
    // Check if any mouse buttons were clicked
    if (!m_mouseButtonsPressed.empty()) {
        return true;
    }
    
    return false;
}

} // namespace clonemine
