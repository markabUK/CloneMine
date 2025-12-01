#include "LoginScreen.h"
#include <iostream>

namespace clonemine {
namespace client {

LoginScreen::LoginScreen() {
    reset();
}

void LoginScreen::reset() {
    m_username.clear();
    m_password.clear();
    m_errorMessage.clear();
    m_usernameFieldActive = true;
    m_errorDisplayTime = 0.0f;
}

void LoginScreen::processInput() {
    // TODO: Integrate with actual input system for text entry
    // For now, this is a stub implementation
    // Real implementation would:
    // - Capture text input for username/password fields
    // - Handle TAB to switch between fields
    // - Handle ENTER to submit login
    // - Handle ESC to clear fields
}

void LoginScreen::update(float deltaTime) {
    // Update error message display time
    if (m_errorDisplayTime > 0.0f) {
        m_errorDisplayTime -= deltaTime;
        if (m_errorDisplayTime <= 0.0f) {
            m_errorMessage.clear();
        }
    }
}

void LoginScreen::render() {
    // TODO: Integrate with actual rendering system
    // This is a stub for the login screen rendering
    // Real implementation would:
    // - Draw title "CloneMine - Login"
    // - Draw username field (with highlight if active)
    // - Draw password field (with asterisks, highlighted if active)
    // - Draw "Login" button
    // - Draw error message if present
    
    std::cout << "=== LOGIN SCREEN ===" << std::endl;
    std::cout << "Username: " << (m_usernameFieldActive ? "[" : "") 
              << m_username << (m_usernameFieldActive ? "]" : "") << std::endl;
    std::cout << "Password: " << (!m_usernameFieldActive ? "[" : "") 
              << std::string(m_password.length(), '*') 
              << (!m_usernameFieldActive ? "]" : "") << std::endl;
    
    if (!m_errorMessage.empty()) {
        std::cout << "ERROR: " << m_errorMessage << std::endl;
    }
    
    std::cout << "Press ENTER to login, TAB to switch fields" << std::endl;
    std::cout << "===================" << std::endl;
}

void LoginScreen::showError(const std::string& error) {
    m_errorMessage = error;
    m_errorDisplayTime = 5.0f; // Display error for 5 seconds
}

void LoginScreen::submitLogin() {
    if (m_username.empty()) {
        showError("Username cannot be empty");
        return;
    }
    
    if (m_password.empty()) {
        showError("Password cannot be empty");
        return;
    }
    
    // Call the login callback if set
    if (m_loginCallback) {
        m_loginCallback(m_username, m_password);
    }
}

} // namespace client
} // namespace clonemine
