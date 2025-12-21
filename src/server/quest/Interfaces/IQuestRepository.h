#ifndef IQUEST_REPOSITORY_H
#define IQUEST_REPOSITORY_H

#include <memory>
#include <vector>
#include <optional>
#include "../Models/Quest.h"
#include "../Models/QuestType.h"

namespace CloneMine {
namespace Quest {

class IQuestRepository {
public:
    virtual ~IQuestRepository() = default;
    
    virtual void AddQuest(const Quest& quest) = 0;
    virtual std::optional<Quest> GetQuest(const std::string& questId) const = 0;
    virtual std::vector<Quest> GetAllQuests() const = 0;
    virtual std::vector<Quest> GetQuestsByType(QuestType type) const = 0;
    virtual bool DeleteQuest(const std::string& questId) = 0;
};

} // namespace Quest
} // namespace CloneMine

#endif
