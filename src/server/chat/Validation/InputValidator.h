#pragma once

#include "../Interfaces/IInputValidator.h"
#include "../Models/ServerConfiguration.h"
#include <regex>
#include <string>
#include <utility>

namespace CloneMine {
namespace Chat {

/**
 * @brief Input validator for chat messages with security checks
 * 
 * Follows Single Responsibility Principle - only validates input
 * Follows Dependency Inversion Principle - implements IInputValidator interface
 */
class InputValidator : public IInputValidator {
private:
    ServerConfiguration config;
    std::regex senderRegex;
    std::regex sqlInjectionRegex;

public:
    explicit InputValidator(const ServerConfiguration& config) 
        : config(config),
          senderRegex("^[a-zA-Z0-9_]{3,16}$"),
          sqlInjectionRegex(R"((--|;|'|\"|\b(OR|AND|SELECT|INSERT|UPDATE|DELETE|DROP|CREATE|ALTER|EXEC|UNION)\b))", 
                           std::regex::icase) {}

    std::pair<bool, std::string> ValidateSender(const std::string& sender) override {
        // Null/empty check
        if (sender.empty()) {
            return {false, "Sender cannot be empty"};
        }

        // Length check
        if (sender.length() < 3 || sender.length() > 16) {
            return {false, "Sender must be 3-16 characters"};
        }

        // Format check (alphanumeric and underscore only)
        if (!std::regex_match(sender, senderRegex)) {
            return {false, "Sender contains invalid characters"};
        }

        // SQL injection check
        if (std::regex_search(sender, sqlInjectionRegex)) {
            return {false, "Sender contains forbidden patterns"};
        }

        // Null byte check
        if (sender.find('\0') != std::string::npos) {
            return {false, "Sender contains null bytes"};
        }

        return {true, ""};
    }

    std::pair<bool, std::string> ValidateMessage(const std::string& message) override {
        // Null/empty check
        if (message.empty()) {
            return {false, "Message cannot be empty"};
        }

        // Length check
        if (static_cast<int>(message.length()) > config.maxMessageLength) {
            return {false, "Message too long (max " + std::to_string(config.maxMessageLength) + " characters)"};
        }

        // SQL injection check
        if (std::regex_search(message, sqlInjectionRegex)) {
            return {false, "Message contains forbidden patterns"};
        }

        // Null byte check
        if (message.find('\0') != std::string::npos) {
            return {false, "Message contains null bytes"};
        }

        return {true, ""};
    }

    std::pair<bool, std::string> ValidateCommand(const std::string& command) override {
        if (command.empty()) {
            return {false, "Command cannot be empty"};
        }

        // Check for buffer overflow attempts
        if (command.length() > 1024) {
            return {false, "Command too long"};
        }

        // Null byte check
        if (command.find('\0') != std::string::npos) {
            return {false, "Command contains null bytes"};
        }

        return {true, ""};
    }
};

} // namespace Chat
} // namespace CloneMine
