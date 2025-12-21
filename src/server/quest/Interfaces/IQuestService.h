#ifndef IQUEST_SERVICE_H
#define IQUEST_SERVICE_H

#include <string>

namespace CloneMine {
namespace Quest {

class IQuestService {
public:
    virtual ~IQuestService() = default;
    
    virtual std::string ListQuests(const std::string& typeFilter) = 0;
    virtual std::string AcceptQuest(const std::string& playerId, const std::string& questId) = 0;
    virtual std::string UpdateProgress(const std::string& playerId, const std::string& questId, double progress) = 0;
    virtual std::string CompleteQuest(const std::string& playerId, const std::string& questId) = 0;
    virtual std::string GetPlayerProgress(const std::string& playerId) = 0;
};

} // namespace Quest
} // namespace CloneMine

#endif
