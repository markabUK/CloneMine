#pragma once

#include "../Interfaces/IInputValidator.h"
#include "../Models/ServerConfiguration.h"
#include <regex>
#include <string>
#include <utility>

namespace CloneMine {
namespace Login {
namespace Validation {

/**
 * @brief Input validator with SQL injection and format checks
 * Compatible with .NET InputValidator for interoperability.
 */
class InputValidator : public IInputValidator {
private:
    ServerConfiguration config_;
    std::regex usernameRegex_;
    std::regex sqlInjectionRegex_;

public:
    explicit InputValidator(const ServerConfiguration& config)
        : config_(config),
          usernameRegex_("^[a-zA-Z0-9_]+$"),
          sqlInjectionRegex_("('|(--|;|/\\*|\\*/|xp_|sp_|exec|execute|select|insert|update|delete|drop|create|alter|union|or\\s+|and\\s+))") {
    }

    std::pair<bool, std::string> ValidateUsername(const std::string& username) override {
        if (username.empty()) {
            return {false, "Username cannot be null or empty"};
        }

        if (username.length() < config_.MinUsernameLength || 
            username.length() > config_.MaxUsernameLength) {
            return {false, "Username must be between " + 
                   std::to_string(config_.MinUsernameLength) + " and " + 
                   std::to_string(config_.MaxUsernameLength) + " characters"};
        }

        if (!std::regex_match(username, usernameRegex_)) {
            return {false, "Username can only contain letters, numbers, and underscores"};
        }

        if (std::regex_search(username, sqlInjectionRegex_)) {
            return {false, "Username contains invalid characters"};
        }

        // Check for null bytes
        if (username.find('\0') != std::string::npos) {
            return {false, "Username contains null bytes"};
        }

        return {true, ""};
    }

    std::pair<bool, std::string> ValidatePassword(const std::string& password) override {
        if (password.empty()) {
            return {false, "Password cannot be null or empty"};
        }

        if (password.length() < config_.MinPasswordLength) {
            return {false, "Password must be at least " + 
                   std::to_string(config_.MinPasswordLength) + " characters"};
        }

        if (password.length() > config_.MaxPasswordLength) {
            return {false, "Password cannot exceed " + 
                   std::to_string(config_.MaxPasswordLength) + " characters"};
        }

        // Check for null bytes
        if (password.find('\0') != std::string::npos) {
            return {false, "Password contains null bytes"};
        }

        return {true, ""};
    }

    std::pair<bool, std::string> ValidateSessionToken(const std::string& token) override {
        if (token.empty()) {
            return {false, "Session token cannot be null or empty"};
        }

        if (token.length() != 32) {
            return {false, "Invalid session token format"};
        }

        // Check if hex string
        for (char c : token) {
            if (!std::isxdigit(c)) {
                return {false, "Session token must be hexadecimal"};
            }
        }

        return {true, ""};
    }

    std::pair<bool, std::string> ValidateMessageLength(size_t length) override {
        if (length > config_.MaxMessageSize) {
            return {false, "Message too large (max " + 
                   std::to_string(config_.MaxMessageSize) + " bytes)"};
        }

        return {true, ""};
    }
};

} // namespace Validation
} // namespace Login
} // namespace CloneMine
