#pragma once

#include "Pathfinding.h"
#include "../combat/DamageTypes.h"
#include <glm/glm.hpp>
#include <vector>
#include <memory>

namespace clonemine {

enum class AIState {
    IDLE,
    ALERT,
    COMBAT,
    FLEEING,
    DEAD
};

struct Ability {
    std::string name;
    float cooldown;
    float lastUsed = -999.0f;
    float range;
    DamageInfo damageInfo;
    
    bool isReady(float currentTime) const {
        return (currentTime - lastUsed) >= cooldown;
    }
};

class MonsterAI {
public:
    MonsterAI(const std::string& monsterType, const glm::vec3& spawnPos)
        : m_type(monsterType), m_position(spawnPos), m_spawnPosition(spawnPos),
          m_state(AIState::IDLE), m_health(100.0f), m_maxHealth(100.0f) {}
    
    virtual ~MonsterAI() = default;
    
    virtual void update(float deltaTime, const glm::vec3& playerPos) {
        m_currentTime += deltaTime;
        
        switch (m_state) {
            case AIState::IDLE:
                updateIdle(deltaTime, playerPos);
                break;
            case AIState::ALERT:
                updateAlert(deltaTime, playerPos);
                break;
            case AIState::COMBAT:
                updateCombat(deltaTime, playerPos);
                break;
            case AIState::FLEEING:
                updateFleeing(deltaTime, playerPos);
                break;
            case AIState::DEAD:
                break;
        }
    }
    
    void takeDamage(float damage) {
        m_health -= damage;
        if (m_health <= 0.0f) {
            m_health = 0.0f;
            m_state = AIState::DEAD;
        } else if (m_health < m_maxHealth * 0.2f && canFlee()) {
            m_state = AIState::FLEEING;
        }
    }
    
    AIState getState() const { return m_state; }
    const glm::vec3& getPosition() const { return m_position; }
    float getHealth() const { return m_health; }
    
    void setAbilities(const std::vector<Ability>& abilities) {
        m_abilities = abilities;
    }
    
    void setResistances(const ResistanceProfile& profile) {
        m_resistances = profile;
    }
    
    const ResistanceProfile& getResistances() const { return m_resistances; }
    
protected:
    virtual void updateIdle(float deltaTime, const glm::vec3& playerPos) {
        float distToPlayer = glm::distance(m_position, playerPos);
        
        if (distToPlayer < m_aggroRange) {
            m_state = AIState::ALERT;
            m_target = playerPos;
            return;
        }
        
        // Wander randomly
        m_wanderTimer -= deltaTime;
        if (m_wanderTimer <= 0.0f) {
            float angle = static_cast<float>(rand()) / RAND_MAX * 2.0f * 3.14159f;
            float dist = 5.0f + static_cast<float>(rand()) / RAND_MAX * 10.0f;
            m_wanderTarget = m_spawnPosition + glm::vec3(cos(angle) * dist, 0, sin(angle) * dist);
            m_wanderTimer = 5.0f;
        }
        
        // Move toward wander target
        glm::vec3 dir = glm::normalize(m_wanderTarget - m_position);
        m_position += dir * m_moveSpeed * deltaTime;
    }
    
    virtual void updateAlert(float deltaTime, const glm::vec3& playerPos) {
        m_target = playerPos;
        float distToPlayer = glm::distance(m_position, playerPos);
        
        if (distToPlayer < m_attackRange) {
            m_state = AIState::COMBAT;
        } else if (distToPlayer > m_aggroRange * 2.0f) {
            m_state = AIState::IDLE;
        } else {
            // Chase player
            glm::vec3 dir = glm::normalize(playerPos - m_position);
            m_position += dir * m_moveSpeed * deltaTime;
        }
    }
    
    virtual void updateCombat(float deltaTime, const glm::vec3& playerPos) {
        m_target = playerPos;
        float distToPlayer = glm::distance(m_position, playerPos);
        
        if (distToPlayer > m_attackRange * 1.5f) {
            m_state = AIState::ALERT;
            return;
        }
        
        // Try to use abilities
        for (auto& ability : m_abilities) {
            if (ability.isReady(m_currentTime) && distToPlayer <= ability.range) {
                useAbility(ability);
                ability.lastUsed = m_currentTime;
                break;
            }
        }
        
        // Move to maintain attack range
        if (distToPlayer > m_attackRange) {
            glm::vec3 dir = glm::normalize(playerPos - m_position);
            m_position += dir * m_moveSpeed * deltaTime;
        } else if (distToPlayer < m_attackRange * 0.5f) {
            glm::vec3 dir = glm::normalize(m_position - playerPos);
            m_position += dir * m_moveSpeed * 0.5f * deltaTime;
        }
    }
    
    virtual void updateFleeing(float deltaTime, const glm::vec3& playerPos) {
        // Run away from player
        glm::vec3 dir = glm::normalize(m_position - playerPos);
        m_position += dir * m_moveSpeed * 1.5f * deltaTime;
        
        float distToPlayer = glm::distance(m_position, playerPos);
        if (distToPlayer > m_aggroRange * 3.0f) {
            m_state = AIState::IDLE;
        }
    }
    
    virtual void useAbility(const Ability& ability) {
        // Override in derived classes for specific behavior
    }
    
    virtual bool canFlee() const {
        return m_fleeThreshold > 0.0f;
    }
    
    std::string m_type;
    glm::vec3 m_position;
    glm::vec3 m_spawnPosition;
    glm::vec3 m_target;
    glm::vec3 m_wanderTarget;
    AIState m_state;
    
    float m_health;
    float m_maxHealth;
    float m_moveSpeed = 3.0f;
    float m_aggroRange = 20.0f;
    float m_attackRange = 2.0f;
    float m_fleeThreshold = 0.2f;
    float m_wanderTimer = 0.0f;
    float m_currentTime = 0.0f;
    
    std::vector<Ability> m_abilities;
    ResistanceProfile m_resistances;
};

} // namespace clonemine
