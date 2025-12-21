#pragma once
#include <string>
#include <utility>

class IInputValidator {
public:
    virtual ~IInputValidator() = default;
    
    virtual std::pair<bool, std::string> ValidateCharacterId(const std::string& id) = 0;
    virtual std::pair<bool, std::string> ValidateCharacterName(const std::string& name) = 0;
    virtual std::pair<bool, std::string> ValidateCharacterClass(const std::string& characterClass) = 0;
    virtual std::pair<bool, std::string> ValidateLevel(int level) = 0;
    virtual std::pair<bool, std::string> ValidateXP(long long xp) = 0;
    virtual std::pair<bool, std::string> ValidateHealth(int health) = 0;
    virtual std::pair<bool, std::string> ValidateMana(int mana) = 0;
};
