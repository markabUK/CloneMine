#ifndef IPROGRESS_REPOSITORY_H
#define IPROGRESS_REPOSITORY_H

#include <memory>
#include <vector>
#include <optional>
#include "../Models/PlayerQuestProgress.h"

namespace CloneMine {
namespace Quest {

class IProgressRepository {
public:
    virtual ~IProgressRepository() = default;
    
    virtual void SaveProgress(const PlayerQuestProgress& progress) = 0;
    virtual std::optional<PlayerQuestProgress> GetProgress(const std::string& playerId, const std::string& questId) const = 0;
    virtual std::vector<PlayerQuestProgress> GetPlayerProgress(const std::string& playerId) const = 0;
    virtual bool DeleteProgress(const std::string& playerId, const std::string& questId) = 0;
};

} // namespace Quest
} // namespace CloneMine

#endif
