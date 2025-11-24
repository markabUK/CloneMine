#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>

namespace clonemine {

enum class Profession {
    BLACKSMITH, WEAPONSMITH, ENCHANTER, ALCHEMIST, MERCHANT,
    QUEST_GIVER, INNKEEPER, GUARD, FARMER, MINER,
    LUMBERJACK, COOK, JEWELER, TAILOR, PRIEST
};

enum class NPCState {
    IDLE, WALKING, WORKING, SOCIALIZING, SLEEPING, FLEEING
};

class VillagerNPC {
public:
    VillagerNPC(const std::string& name, Profession profession, const glm::vec3& homePosition);
    
    void update(float deltaTime, float timeOfDay);
    
    bool canInteract() const;
    std::string getGreeting() const;
    void onInteract();
    
    glm::vec3 getPosition() const { return m_position; }
    void setPosition(const glm::vec3& pos) { m_position = pos; }
    
    std::string getName() const { return m_name; }
    Profession getProfession() const { return m_profession; }
    NPCState getState() const { return m_state; }
    
    void updateSchedule(float timeOfDay);
    
private:
    std::string m_name;
    Profession m_profession;
    glm::vec3 m_position, m_homePosition, m_workPosition, m_destination;
    
    NPCState m_state{NPCState::IDLE};
    float m_moveSpeed{2.0f};
    float m_stateTimer{0.0f};
    bool m_isAtWork{false}, m_isAtHome{false};
    
    void moveTowards(const glm::vec3& target, float deltaTime);
    void updateIdleBehavior(float deltaTime);
    void updateWorkingBehavior(float deltaTime);
    std::string getProfessionName() const;
};

} // namespace clonemine
