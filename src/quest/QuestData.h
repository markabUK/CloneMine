#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <glm/glm.hpp>

namespace clonemine {
namespace quest {

enum class QuestType {
    KILL_MONSTERS,
    COLLECT_ITEMS,
    EXPLORE_LOCATION,
    TALK_TO_NPC,
    CRAFT_ITEM,
    COMBINATION
};

enum class QuestStatus {
    AVAILABLE,
    IN_PROGRESS,
    COMPLETED,
    FAILED
};

struct QuestObjective {
    std::string description;
    QuestType type;
    std::string targetId;      // Monster name, item name, location name, etc.
    uint32_t requiredAmount;
    uint32_t currentAmount{0};
    
    [[nodiscard]] bool isComplete() const {
        return currentAmount >= requiredAmount;
    }
    
    [[nodiscard]] float getProgress() const {
        return static_cast<float>(currentAmount) / static_cast<float>(requiredAmount);
    }
};

struct QuestReward {
    uint32_t experience{0};
    uint32_t gold{0};
    std::vector<std::string> items;
};

struct Quest {
    uint32_t questId;
    std::string title;
    std::string description;
    uint32_t requiredLevel;
    uint32_t recommendedLevel;
    
    std::vector<QuestObjective> objectives;
    QuestReward rewards;
    
    QuestStatus status{QuestStatus::AVAILABLE};
    
    [[nodiscard]] bool isComplete() const {
        for (const auto& obj : objectives) {
            if (!obj.isComplete()) {
                return false;
            }
        }
        return true;
    }
    
    [[nodiscard]] float getOverallProgress() const {
        if (objectives.empty()) return 0.0f;
        
        float total = 0.0f;
        for (const auto& obj : objectives) {
            total += obj.getProgress();
        }
        return total / static_cast<float>(objectives.size());
    }
};

struct QuestProgress {
    uint32_t questId;
    uint32_t playerId;
    std::vector<uint32_t> objectiveProgress; // Progress for each objective
};

} // namespace quest
} // namespace clonemine
