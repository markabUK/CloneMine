#pragma once

#include <string>
#include <functional>

namespace clonemine {
namespace client {

class LoginScreen {
public:
    LoginScreen();
    ~LoginScreen() = default;

    // Process input for the login screen
    void processInput();
    
    // Update login screen state
    void update(float deltaTime);
    
    // Render the login screen
    void render();
    
    // Set callback for successful login
    void setLoginCallback(std::function<void(const std::string&, const std::string&)> callback) {
        m_loginCallback = callback;
    }
    
    // Get entered username
    [[nodiscard]] const std::string& getUsername() const { return m_username; }
    
    // Get entered password
    [[nodiscard]] const std::string& getPassword() const { return m_password; }
    
    // Reset the screen
    void reset();
    
    // Display error message
    void showError(const std::string& error);

private:
    std::string m_username;
    std::string m_password;
    std::string m_errorMessage;
    bool m_usernameFieldActive{true}; // true = username field, false = password field
    float m_errorDisplayTime{0.0f};
    
    std::function<void(const std::string&, const std::string&)> m_loginCallback;
    
    void submitLogin();
};

} // namespace client
} // namespace clonemine
