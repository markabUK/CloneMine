#ifndef QUEST_TYPE_H
#define QUEST_TYPE_H

#include <string>

namespace CloneMine {
namespace Quest {

// Quest type enumeration
enum class QuestType {
    MAIN = 0,
    SIDE = 1,
    DAILY = 2,
    REPEATABLE = 3
};

// Convert QuestType to string
inline std::string QuestTypeToString(QuestType type) {
    switch (type) {
        case QuestType::MAIN: return "MAIN";
        case QuestType::SIDE: return "SIDE";
        case QuestType::DAILY: return "DAILY";
        case QuestType::REPEATABLE: return "REPEATABLE";
        default: return "UNKNOWN";
    }
}

// Convert string to QuestType
inline QuestType StringToQuestType(const std::string& str) {
    if (str == "MAIN") return QuestType::MAIN;
    if (str == "SIDE") return QuestType::SIDE;
    if (str == "DAILY") return QuestType::DAILY;
    if (str == "REPEATABLE") return QuestType::REPEATABLE;
    return QuestType::MAIN;  // Default
}

} // namespace Quest
} // namespace CloneMine

#endif // QUEST_TYPE_H
