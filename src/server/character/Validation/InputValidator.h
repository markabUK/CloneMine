#pragma once
#include "../Interfaces/IInputValidator.h"
#include "../Models/ServerConfiguration.h"
#include <regex>

class InputValidator : public IInputValidator {
private:
    ServerConfiguration config;
    std::regex characterIdRegex;
    std::regex characterNameRegex;
    std::regex characterClassRegex;
    std::regex sqlInjectionRegex;
    
public:
    InputValidator(const ServerConfiguration& cfg)
        : config(cfg),
          characterIdRegex("^[a-zA-Z0-9_-]{3,64}$"),
          characterNameRegex("^[a-zA-Z0-9 ]{3,32}$"),
          characterClassRegex("^[a-zA-Z]{3,20}$"),
          sqlInjectionRegex("(--|;|'|\"|/\\*|\\*\\/|xp_|sp_|exec|union|select|insert|update|delete|drop|create|alter)") {}
    
    std::pair<bool, std::string> ValidateCharacterId(const std::string& id) override {
        if (id.empty()) {
            return {false, "Character ID cannot be empty"};
        }
        if (id.length() < 3 || id.length() > 64) {
            return {false, "Character ID must be 3-64 characters"};
        }
        if (!std::regex_match(id, characterIdRegex)) {
            return {false, "Character ID contains invalid characters"};
        }
        if (std::regex_search(id, sqlInjectionRegex)) {
            return {false, "Character ID contains SQL keywords"};
        }
        return {true, ""};
    }
    
    std::pair<bool, std::string> ValidateCharacterName(const std::string& name) override {
        if (name.empty()) {
            return {false, "Character name cannot be empty"};
        }
        if (name.length() < 3 || name.length() > 32) {
            return {false, "Character name must be 3-32 characters"};
        }
        if (!std::regex_match(name, characterNameRegex)) {
            return {false, "Character name contains invalid characters"};
        }
        if (std::regex_search(name, sqlInjectionRegex)) {
            return {false, "Character name contains SQL keywords"};
        }
        return {true, ""};
    }
    
    std::pair<bool, std::string> ValidateCharacterClass(const std::string& characterClass) override {
        if (characterClass.empty()) {
            return {false, "Character class cannot be empty"};
        }
        if (characterClass.length() < 3 || characterClass.length() > 20) {
            return {false, "Character class must be 3-20 characters"};
        }
        if (!std::regex_match(characterClass, characterClassRegex)) {
            return {false, "Character class must contain only letters"};
        }
        return {true, ""};
    }
    
    std::pair<bool, std::string> ValidateLevel(int level) override {
        if (level < 1 || level > 100) {
            return {false, "Level must be between 1 and 100"};
        }
        return {true, ""};
    }
    
    std::pair<bool, std::string> ValidateXP(long long xp) override {
        if (xp < 0 || xp > 999999999) {
            return {false, "XP must be between 0 and 999999999"};
        }
        return {true, ""};
    }
    
    std::pair<bool, std::string> ValidateHealth(int health) override {
        if (health < 0 || health > 999999) {
            return {false, "Health must be between 0 and 999999"};
        }
        return {true, ""};
    }
    
    std::pair<bool, std::string> ValidateMana(int mana) override {
        if (mana < 0 || mana > 999999) {
            return {false, "Mana must be between 0 and 999999"};
        }
        return {true, ""};
    }
};
