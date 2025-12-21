#ifndef QUEST_IINPUT_VALIDATOR_H
#define QUEST_IINPUT_VALIDATOR_H

#include <string>
#include <utility>

namespace CloneMine {
namespace Quest {

class IInputValidator {
public:
    virtual ~IInputValidator() = default;
    
    virtual std::pair<bool, std::string> ValidateQuestId(const std::string& questId) const = 0;
    virtual std::pair<bool, std::string> ValidatePlayerId(const std::string& playerId) const = 0;
    virtual std::pair<bool, std::string> ValidateProgress(double progress) const = 0;
};

} // namespace Quest
} // namespace CloneMine

#endif
