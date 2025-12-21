#pragma once

#include "../Interfaces/IMessageHandler.h"
#include "../Interfaces/IAuthenticationService.h"
#include "../Interfaces/IInputValidator.h"
#include <memory>
#include <string>
#include <vector>
#include <sstream>

namespace CloneMine {
namespace Login {
namespace Handlers {

/**
 * @brief Login message handler
 * Processes login protocol messages.
 */
class LoginMessageHandler : public IMessageHandler {
private:
    std::shared_ptr<IAuthenticationService> authService_;
    std::shared_ptr<IInputValidator> inputValidator_;

    /**
     * @brief Parse message into command and parameters
     * Format: "COMMAND param1 param2 ..."
     */
    std::vector<std::string> ParseMessage(const std::string& message) {
        std::vector<std::string> parts;
        std::istringstream iss(message);
        std::string part;
        
        while (iss >> part) {
            parts.push_back(part);
        }
        
        return parts;
    }

    /**
     * @brief Handle LOGIN command
     * Format: "LOGIN username password"
     */
    std::string HandleLogin(const std::vector<std::string>& parts) {
        if (parts.size() != 3) {
            return "ERROR Invalid login format. Usage: LOGIN username password";
        }

        const std::string& username = parts[1];
        const std::string& password = parts[2];

        auto result = authService_->Login(username, password);
        
        if (result.Success) {
            return "SUCCESS " + result.SessionToken + " " + std::to_string(result.Account->Id);
        }
        else {
            return "ERROR " + result.Message;
        }
    }

    /**
     * @brief Handle REGISTER command
     * Format: "REGISTER username password"
     */
    std::string HandleRegister(const std::vector<std::string>& parts) {
        if (parts.size() != 3) {
            return "ERROR Invalid register format. Usage: REGISTER username password";
        }

        const std::string& username = parts[1];
        const std::string& password = parts[2];

        auto result = authService_->Register(username, password);
        
        if (result.Success) {
            return "SUCCESS " + result.SessionToken + " " + std::to_string(result.Account->Id);
        }
        else {
            return "ERROR " + result.Message;
        }
    }

    /**
     * @brief Handle LOGOUT command
     * Format: "LOGOUT sessionToken"
     */
    std::string HandleLogout(const std::vector<std::string>& parts) {
        if (parts.size() != 2) {
            return "ERROR Invalid logout format. Usage: LOGOUT sessionToken";
        }

        const std::string& sessionToken = parts[1];

        if (authService_->Logout(sessionToken)) {
            return "SUCCESS Logged out";
        }
        else {
            return "ERROR Logout failed";
        }
    }

    /**
     * @brief Handle PING command
     * Format: "PING"
     */
    std::string HandlePing(const std::vector<std::string>& parts) {
        return "PONG";
    }

public:
    LoginMessageHandler(std::shared_ptr<IAuthenticationService> authService,
                       std::shared_ptr<IInputValidator> inputValidator)
        : authService_(authService),
          inputValidator_(inputValidator) {
    }

    std::string HandleMessage(const std::string& message) override {
        if (message.empty()) {
            return "ERROR Empty message";
        }

        // Parse message
        auto parts = ParseMessage(message);
        if (parts.empty()) {
            return "ERROR Invalid message format";
        }

        const std::string& command = parts[0];

        // Route to appropriate handler
        if (command == "LOGIN") {
            return HandleLogin(parts);
        }
        else if (command == "REGISTER") {
            return HandleRegister(parts);
        }
        else if (command == "LOGOUT") {
            return HandleLogout(parts);
        }
        else if (command == "PING") {
            return HandlePing(parts);
        }
        else {
            return "ERROR Unknown command: " + command;
        }
    }
};

} // namespace Handlers
} // namespace Login
} // namespace CloneMine
