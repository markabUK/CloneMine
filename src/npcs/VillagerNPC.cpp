#include "VillagerNPC.h"
#include <cmath>

namespace clonemine {

VillagerNPC::VillagerNPC(const std::string& name, Profession profession, const glm::vec3& homePosition)
    : m_name(name), m_profession(profession), m_position(homePosition), 
      m_homePosition(homePosition), m_workPosition(homePosition) {
    m_workPosition.x += 20.0f;
}

void VillagerNPC::update(float deltaTime, float timeOfDay) {
    updateSchedule(timeOfDay);
    
    switch (m_state) {
        case NPCState::IDLE:
            updateIdleBehavior(deltaTime);
            break;
        case NPCState::WALKING:
            moveTowards(m_destination, deltaTime);
            if (glm::distance(m_position, m_destination) < 1.0f) {
                m_state = m_isAtWork ? NPCState::WORKING : NPCState::IDLE;
            }
            break;
        case NPCState::WORKING:
            updateWorkingBehavior(deltaTime);
            break;
        default:
            break;
    }
}

bool VillagerNPC::canInteract() const {
    return m_state != NPCState::SLEEPING && m_state != NPCState::FLEEING;
}

std::string VillagerNPC::getGreeting() const {
    return "Hello, traveler! I am " + m_name + ", the " + getProfessionName() + ".";
}

void VillagerNPC::onInteract() {
    // Interaction logic
}

void VillagerNPC::updateSchedule(float timeOfDay) {
    if (timeOfDay >= 6.0f && timeOfDay < 12.0f) {
        if (!m_isAtWork && m_state != NPCState::WALKING) {
            m_destination = m_workPosition;
            m_state = NPCState::WALKING;
            m_isAtWork = true;
            m_isAtHome = false;
        }
    } else if (timeOfDay >= 18.0f && timeOfDay < 22.0f) {
        if (!m_isAtHome && m_state != NPCState::WALKING) {
            m_destination = m_homePosition;
            m_state = NPCState::WALKING;
            m_isAtHome = true;
            m_isAtWork = false;
        }
    } else if (timeOfDay >= 22.0f || timeOfDay < 6.0f) {
        if (m_isAtHome) {
            m_state = NPCState::SLEEPING;
        }
    }
}

void VillagerNPC::moveTowards(const glm::vec3& target, float deltaTime) {
    glm::vec3 direction = target - m_position;
    float distance = glm::length(direction);
    
    if (distance > 0.1f) {
        direction = glm::normalize(direction);
        m_position += direction * m_moveSpeed * deltaTime;
    }
}

void VillagerNPC::updateIdleBehavior(float deltaTime) {
    m_stateTimer += deltaTime;
    if (m_stateTimer > 5.0f) {
        m_stateTimer = 0.0f;
        m_destination = m_position;
        m_destination.x += (rand() % 20) - 10;
        m_destination.z += (rand() % 20) - 10;
        m_state = NPCState::WALKING;
    }
}

void VillagerNPC::updateWorkingBehavior(float deltaTime) {
    m_stateTimer += deltaTime;
    if (m_stateTimer > 10.0f) {
        m_stateTimer = 0.0f;
        m_state = NPCState::IDLE;
    }
}

std::string VillagerNPC::getProfessionName() const {
    switch (m_profession) {
        case Profession::BLACKSMITH: return "Blacksmith";
        case Profession::WEAPONSMITH: return "Weaponsmith";
        case Profession::ENCHANTER: return "Enchanter";
        case Profession::ALCHEMIST: return "Alchemist";
        case Profession::MERCHANT: return "Merchant";
        case Profession::QUEST_GIVER: return "Quest Giver";
        case Profession::INNKEEPER: return "Innkeeper";
        case Profession::GUARD: return "Guard";
        case Profession::FARMER: return "Farmer";
        case Profession::MINER: return "Miner";
        case Profession::LUMBERJACK: return "Lumberjack";
        case Profession::COOK: return "Cook";
        case Profession::JEWELER: return "Jeweler";
        case Profession::TAILOR: return "Tailor";
        case Profession::PRIEST: return "Priest";
        default: return "Villager";
    }
}

} // namespace clonemine
