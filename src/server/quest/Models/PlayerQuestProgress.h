#ifndef PLAYER_QUEST_PROGRESS_H
#define PLAYER_QUEST_PROGRESS_H

#include <string>

namespace CloneMine {
namespace Quest {

class PlayerQuestProgress {
public:
    std::string playerId;
    std::string questId;
    double progress;
    std::string status;
    
    PlayerQuestProgress() : progress(0.0), status("NOT_STARTED") {}
    
    PlayerQuestProgress(const std::string& playerId, const std::string& questId,
                       double progress, const std::string& status)
        : playerId(playerId), questId(questId), progress(progress), status(status) {}
    
    std::string ToString() const {
        return questId + " " + std::to_string(progress) + " " + status;
    }
};

} // namespace Quest
} // namespace CloneMine

#endif
