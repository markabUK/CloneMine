#ifndef IN_MEMORY_PROGRESS_REPOSITORY_H
#define IN_MEMORY_PROGRESS_REPOSITORY_H

#include <map>
#include <vector>
#include <mutex>
#include "../Interfaces/IProgressRepository.h"

namespace CloneMine {
namespace Quest {

class InMemoryProgressRepository : public IProgressRepository {
private:
    std::map<std::string, PlayerQuestProgress> progress;  // Key: playerId_questId
    mutable std::mutex mutex;
    
    std::string MakeKey(const std::string& playerId, const std::string& questId) const {
        return playerId + "_" + questId;
    }
    
public:
    void SaveProgress(const PlayerQuestProgress& prog) override {
        std::lock_guard<std::mutex> lock(mutex);
        std::string key = MakeKey(prog.playerId, prog.questId);
        progress[key] = prog;
    }
    
    std::optional<PlayerQuestProgress> GetProgress(const std::string& playerId, 
                                                   const std::string& questId) const override {
        std::lock_guard<std::mutex> lock(mutex);
        std::string key = MakeKey(playerId, questId);
        auto it = progress.find(key);
        if (it != progress.end()) {
            return it->second;
        }
        return std::nullopt;
    }
    
    std::vector<PlayerQuestProgress> GetPlayerProgress(const std::string& playerId) const override {
        std::lock_guard<std::mutex> lock(mutex);
        std::vector<PlayerQuestProgress> result;
        for (const auto& pair : progress) {
            if (pair.second.playerId == playerId) {
                result.push_back(pair.second);
            }
        }
        return result;
    }
    
    bool DeleteProgress(const std::string& playerId, const std::string& questId) override {
        std::lock_guard<std::mutex> lock(mutex);
        std::string key = MakeKey(playerId, questId);
        return progress.erase(key) > 0;
    }
};

} // namespace Quest
} // namespace CloneMine

#endif
