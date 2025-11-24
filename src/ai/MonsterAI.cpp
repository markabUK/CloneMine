#include "MonsterAI.h"
#include "../world/World.h"
#include "../core/Random.h"
#include <cmath>

namespace clonemine {

MonsterAI::MonsterAI(Entity* entity, const AIConfig& config)
    : m_entity(entity)
    , m_config(config)
    , m_state(AIState::IDLE)
    , m_target(nullptr)
    , m_stateTimer(0.0f)
    , m_abilityTimer(0.0f)
{
}

void MonsterAI::update(float deltaTime, World& world) {
    m_stateTimer += deltaTime;
    m_abilityTimer += deltaTime;
    
    switch (m_state) {
        case AIState::IDLE:
            updateIdle(deltaTime, world);
            break;
        case AIState::ALERT:
            updateAlert(deltaTime, world);
            break;
        case AIState::COMBAT:
            updateCombat(deltaTime, world);
            break;
        case AIState::FLEEING:
            updateFleeing(deltaTime, world);
            break;
    }
}

void MonsterAI::updateIdle(float deltaTime, World& world) {
    // Detect nearby players
    m_target = findNearestPlayer(world, m_config.aggroRange);
    
    if (m_target) {
        setState(AIState::ALERT);
        return;
    }
    
    // Random wandering
    if (m_stateTimer > 5.0f) {
        glm::vec3 randomOffset(
            Random::range(-5.0f, 5.0f),
            0.0f,
            Random::range(-5.0f, 5.0f)
        );
        m_wanderTarget = m_entity->getPosition() + randomOffset;
        m_stateTimer = 0.0f;
    }
    
    // Move toward wander target
    if (glm::distance(m_entity->getPosition(), m_wanderTarget) > 0.5f) {
        glm::vec3 direction = glm::normalize(m_wanderTarget - m_entity->getPosition());
        m_entity->move(direction * m_config.moveSpeed * deltaTime);
    }
}

void MonsterAI::updateAlert(float deltaTime, World& world) {
    if (!m_target || !isTargetValid()) {
        setState(AIState::IDLE);
        return;
    }
    
    float distanceToTarget = glm::distance(m_entity->getPosition(), m_target->getPosition());
    
    // Enter combat if in range
    if (distanceToTarget <= m_config.attackRange) {
        setState(AIState::COMBAT);
        return;
    }
    
    // Move toward target
    moveToward(m_target->getPosition(), deltaTime);
}

void MonsterAI::updateCombat(float deltaTime, World& world) {
    if (!m_target || !isTargetValid()) {
        setState(AIState::IDLE);
        return;
    }
    
    // Check flee threshold
    float healthPercent = m_entity->getHealth() / m_entity->getMaxHealth();
    if (healthPercent < m_config.fleeThreshold && m_config.fleeThreshold > 0.0f) {
        setState(AIState::FLEEING);
        return;
    }
    
    float distanceToTarget = glm::distance(m_entity->getPosition(), m_target->getPosition());
    
    // Move to attack range if too far
    if (distanceToTarget > m_config.attackRange) {
        moveToward(m_target->getPosition(), deltaTime);
        return;
    }
    
    // Face target
    glm::vec3 directionToTarget = glm::normalize(m_target->getPosition() - m_entity->getPosition());
    m_entity->setRotation(std::atan2(directionToTarget.z, directionToTarget.x));
    
    // Use abilities
    if (m_abilityTimer >= m_config.abilities[m_currentAbility].cooldown) {
        useAbility(m_config.abilities[m_currentAbility], world);
        m_abilityTimer = 0.0f;
        m_currentAbility = (m_currentAbility + 1) % m_config.abilities.size();
    }
}

void MonsterAI::updateFleeing(float deltaTime, World& world) {
    if (!m_target) {
        setState(AIState::IDLE);
        return;
    }
    
    // Run away from target
    glm::vec3 fleeDirection = glm::normalize(m_entity->getPosition() - m_target->getPosition());
    m_entity->move(fleeDirection * m_config.moveSpeed * 1.5f * deltaTime);
    
    // Stop fleeing if far enough away
    float distanceToTarget = glm::distance(m_entity->getPosition(), m_target->getPosition());
    if (distanceToTarget > m_config.aggroRange * 2.0f) {
        setState(AIState::IDLE);
    }
}

void MonsterAI::setState(AIState newState) {
    m_state = newState;
    m_stateTimer = 0.0f;
}

Entity* MonsterAI::findNearestPlayer(World& world, float range) {
    Entity* nearest = nullptr;
    float nearestDistance = range;
    
    for (auto& player : world.getPlayers()) {
        float distance = glm::distance(m_entity->getPosition(), player->getPosition());
        if (distance < nearestDistance) {
            nearest = player.get();
            nearestDistance = distance;
        }
    }
    
    return nearest;
}

bool MonsterAI::isTargetValid() const {
    if (!m_target) return false;
    
    // Check if target is alive
    if (m_target->getHealth() <= 0.0f) return false;
    
    // Check if target is too far away (leash)
    float distance = glm::distance(m_entity->getPosition(), m_target->getPosition());
    if (distance > m_config.aggroRange * 3.0f) return false;
    
    return true;
}

void MonsterAI::moveToward(const glm::vec3& target, float deltaTime) {
    glm::vec3 direction = glm::normalize(target - m_entity->getPosition());
    m_entity->move(direction * m_config.moveSpeed * deltaTime);
}

void MonsterAI::useAbility(const Ability& ability, World& world) {
    // Play animation
    m_entity->playAnimation(ability.animationName);
    
    // Apply ability effect
    if (ability.type == AbilityType::MELEE_ATTACK) {
        if (m_target) {
            // Calculate damage
            float damage = ability.baseDamage;
            m_target->takeDamage(damage, ability.damageType);
        }
    } else if (ability.type == AbilityType::RANGED_ATTACK) {
        if (m_target) {
            // Spawn projectile
            world.spawnProjectile(m_entity->getPosition(), m_target->getPosition(),
                                 ability.baseDamage, ability.damageType);
        }
    } else if (ability.type == AbilityType::AOE_ATTACK) {
        // Damage all nearby entities
        auto nearbyEntities = world.getEntitiesInRadius(m_entity->getPosition(), ability.range);
        for (auto entity : nearbyEntities) {
            if (entity != m_entity && entity->isPlayer()) {
                entity->takeDamage(ability.baseDamage, ability.damageType);
            }
        }
    } else if (ability.type == AbilityType::BUFF) {
        // Apply buff to self
        m_entity->applyBuff(ability.buffType, ability.duration);
    } else if (ability.type == AbilityType::HEAL) {
        // Heal self or ally
        m_entity->heal(ability.baseDamage);
    }
}

void MonsterAI::onDamageTaken(Entity* attacker) {
    // Aggro on attacker if not already in combat
    if (m_state == AIState::IDLE) {
        m_target = attacker;
        setState(AIState::ALERT);
    }
}

} // namespace clonemine
