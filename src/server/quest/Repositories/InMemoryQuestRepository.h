#ifndef IN_MEMORY_QUEST_REPOSITORY_H
#define IN_MEMORY_QUEST_REPOSITORY_H

#include <map>
#include <vector>
#include <mutex>
#include "../Interfaces/IQuestRepository.h"

namespace CloneMine {
namespace Quest {

class InMemoryQuestRepository : public IQuestRepository {
private:
    std::map<std::string, Quest> quests;
    mutable std::mutex mutex;
    
public:
    InMemoryQuestRepository() {
        // Pre-load default quests
        AddQuest(Quest("quest1", "Defeat_the_Boss", "Kill_the_dragon_boss", 100, QuestType::MAIN));
        AddQuest(Quest("quest2", "Collect_Herbs", "Gather_10_herbs", 50, QuestType::SIDE));
        AddQuest(Quest("quest3", "Daily_Login", "Log_in_for_daily_rewards", 10, QuestType::DAILY));
    }
    
    void AddQuest(const Quest& quest) override {
        std::lock_guard<std::mutex> lock(mutex);
        quests[quest.id] = quest;
    }
    
    std::optional<Quest> GetQuest(const std::string& questId) const override {
        std::lock_guard<std::mutex> lock(mutex);
        auto it = quests.find(questId);
        if (it != quests.end()) {
            return it->second;
        }
        return std::nullopt;
    }
    
    std::vector<Quest> GetAllQuests() const override {
        std::lock_guard<std::mutex> lock(mutex);
        std::vector<Quest> result;
        for (const auto& pair : quests) {
            result.push_back(pair.second);
        }
        return result;
    }
    
    std::vector<Quest> GetQuestsByType(QuestType type) const override {
        std::lock_guard<std::mutex> lock(mutex);
        std::vector<Quest> result;
        for (const auto& pair : quests) {
            if (pair.second.type == type) {
                result.push_back(pair.second);
            }
        }
        return result;
    }
    
    bool DeleteQuest(const std::string& questId) override {
        std::lock_guard<std::mutex> lock(mutex);
        return quests.erase(questId) > 0;
    }
};

} // namespace Quest
} // namespace CloneMine

#endif
