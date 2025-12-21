#ifndef QUEST_H
#define QUEST_H

#include <string>
#include "QuestType.h"

namespace CloneMine {
namespace Quest {

class Quest {
public:
    std::string id;
    std::string name;
    std::string description;
    int reward;
    QuestType type;
    
    Quest() : reward(0), type(QuestType::MAIN) {}
    
    Quest(const std::string& id, const std::string& name, const std::string& description, 
          int reward, QuestType type)
        : id(id), name(name), description(description), reward(reward), type(type) {}
    
    std::string ToString() const {
        return id + " " + name + " " + description + " " + 
               std::to_string(reward) + " " + QuestTypeToString(type);
    }
};

} // namespace Quest
} // namespace CloneMine

#endif
