#pragma once

#include "../Interfaces/IInputValidator.h"
#include "../Models/ServerConfiguration.h"
#include <regex>
#include <string>
#include <memory>
#include <cmath>

namespace CloneMine {
namespace Game {
namespace Validation {

class InputValidator : public Interfaces::IInputValidator {
private:
    std::shared_ptr<Models::ServerConfiguration> config;
    std::regex playerIdRegex;
    std::regex usernameRegex;
    std::regex sqlInjectionRegex;

public:
    InputValidator(std::shared_ptr<Models::ServerConfiguration> configuration)
        : config(configuration),
          playerIdRegex("^[a-fA-F0-9]{32}$"),  // 128-bit hex
          usernameRegex("^[a-zA-Z0-9_]{3,16}$"),
          sqlInjectionRegex("('|(--|;|\\||\\*|<|>|=|xp_|sp_|exec|execute|select|insert|update|delete|drop|create|alter|union|script|javascript|onerror|onload))") {
    }

    std::pair<bool, std::string> ValidatePlayerId(const std::string& playerId) override {
        if (playerId.empty()) {
            return {false, "Player ID cannot be empty"};
        }
        
        if (playerId.find('\0') != std::string::npos) {
            return {false, "Player ID contains null bytes"};
        }
        
        if (!std::regex_match(playerId, playerIdRegex)) {
            return {false, "Player ID must be 32-character hex string"};
        }
        
        if (std::regex_search(playerId, sqlInjectionRegex)) {
            return {false, "Player ID contains invalid characters"};
        }
        
        return {true, ""};
    }

    std::pair<bool, std::string> ValidateUsername(const std::string& username) override {
        if (username.empty()) {
            return {false, "Username cannot be empty"};
        }
        
        if (username.length() < 3 || username.length() > 16) {
            return {false, "Username must be 3-16 characters"};
        }
        
        if (username.find('\0') != std::string::npos) {
            return {false, "Username contains null bytes"};
        }
        
        if (!std::regex_match(username, usernameRegex)) {
            return {false, "Username can only contain letters, numbers, and underscores"};
        }
        
        if (std::regex_search(username, sqlInjectionRegex)) {
            return {false, "Username contains invalid characters"};
        }
        
        return {true, ""};
    }

    std::pair<bool, std::string> ValidateCoordinate(double coordinate) override {
        if (std::isnan(coordinate) || std::isinf(coordinate)) {
            return {false, "Coordinate is not a valid number"};
        }
        
        if (std::abs(coordinate) > config->MaxCoordinateValue) {
            return {false, "Coordinate exceeds maximum allowed value"};
        }
        
        return {true, ""};
    }

    std::pair<bool, std::string> ValidateCoordinates(double x, double y, double z) override {
        auto xResult = ValidateCoordinate(x);
        if (!xResult.first) {
            return {false, "X " + xResult.second};
        }
        
        auto yResult = ValidateCoordinate(y);
        if (!yResult.first) {
            return {false, "Y " + yResult.second};
        }
        
        auto zResult = ValidateCoordinate(z);
        if (!zResult.first) {
            return {false, "Z " + zResult.second};
        }
        
        return {true, ""};
    }
};

} // namespace Validation
} // namespace Game
} // namespace CloneMine
