#ifndef QUEST_INPUT_VALIDATOR_H
#define QUEST_INPUT_VALIDATOR_H

#include <string>
#include <regex>
#include <utility>
#include "../Interfaces/IInputValidator.h"

namespace CloneMine {
namespace Quest {

class InputValidator : public IInputValidator {
private:
    std::regex questIdRegex;
    std::regex playerIdRegex;
    std::regex sqlInjectionRegex;
    
public:
    InputValidator() {
        questIdRegex = std::regex("^[a-zA-Z0-9_-]{3,32}$");
        playerIdRegex = std::regex("^[a-zA-Z0-9_-]{3,32}$");
        sqlInjectionRegex = std::regex("('|(--|;|/\\*|\\*/|xp_|sp_|exec|execute|select|insert|update|delete|drop|create|alter))");
    }
    
    std::pair<bool, std::string> ValidateQuestId(const std::string& questId) const override {
        if (questId.empty()) return {false, "Quest ID cannot be empty"};
        if (questId.length() < 3 || questId.length() > 32) return {false, "Invalid quest ID length"};
        if (!std::regex_match(questId, questIdRegex)) return {false, "Invalid quest ID format"};
        if (std::regex_search(questId, sqlInjectionRegex)) return {false, "Invalid characters in quest ID"};
        return {true, ""};
    }
    
    std::pair<bool, std::string> ValidatePlayerId(const std::string& playerId) const override {
        if (playerId.empty()) return {false, "Player ID cannot be empty"};
        if (playerId.length() < 3 || playerId.length() > 32) return {false, "Invalid player ID length"};
        if (!std::regex_match(playerId, playerIdRegex)) return {false, "Invalid player ID format"};
        if (std::regex_search(playerId, sqlInjectionRegex)) return {false, "Invalid characters in player ID"};
        return {true, ""};
    }
    
    std::pair<bool, std::string> ValidateProgress(double progress) const override {
        if (progress < 0.0 || progress > 100.0) return {false, "Progress must be between 0 and 100"};
        return {true, ""};
    }
};

} // namespace Quest
} // namespace CloneMine

#endif
