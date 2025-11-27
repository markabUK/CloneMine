#include "CombatSystem.h"
#include <random>
#include <cmath>
#include <iostream>

namespace clonemine {

CombatSystem::CombatSystem() {
    std::cout << "[CombatSystem] Initialized PvE combat system with EntityId support" << std::endl;
}

CombatSystem::~CombatSystem() = default;

void CombatSystem::update(float deltaTime) {
    auto currentTime = std::chrono::system_clock::now();
    
    for (auto& [entityId, data] : m_entityCombatData) {
        // Update ability cooldowns
        for (auto& [slot, cooldown] : data.abilityCooldowns) {
            if (cooldown > 0.0f) {
                cooldown -= deltaTime;
            }
        }

        // Auto-attack processing
        if (data.autoAttacking && data.targetId.isValid()) {
            if (processAutoAttack(entityId, deltaTime)) {
                data.lastCombatTime = std::chrono::duration<float>(
                    currentTime.time_since_epoch()).count();
            }
        }

        // Combat timeout check
        if (data.state == CombatState::IN_COMBAT) {
            float timeSinceLastCombat = std::chrono::duration<float>(
                currentTime.time_since_epoch()).count() - data.lastCombatTime;
            
            if (timeSinceLastCombat >= COMBAT_TIMEOUT) {
                leaveCombat(entityId);
            }
        }
    }
}

void CombatSystem::enterCombat(const EntityId& entityId) {
    auto& data = m_entityCombatData[entityId];
    if (data.state != CombatState::IN_COMBAT) {
        data.state = CombatState::IN_COMBAT;
        data.lastCombatTime = std::chrono::duration<float>(
            std::chrono::system_clock::now().time_since_epoch()).count();
        std::cout << "[Combat] " << entityTypeToString(entityId.type) << " '" << entityId.name << "' entered combat" << std::endl;
    }
}

void CombatSystem::leaveCombat(const EntityId& entityId) {
    auto it = m_entityCombatData.find(entityId);
    if (it != m_entityCombatData.end()) {
        it->second.state = CombatState::OUT_OF_COMBAT;
        it->second.autoAttacking = false;
        std::cout << "[Combat] " << entityTypeToString(entityId.type) << " '" << entityId.name << "' left combat" << std::endl;
    }
}

bool CombatSystem::isInCombat(const EntityId& entityId) const {
    auto it = m_entityCombatData.find(entityId);
    return it != m_entityCombatData.end() && it->second.state == CombatState::IN_COMBAT;
}

void CombatSystem::setTarget(const EntityId& attackerId, const EntityId& targetId) {
    m_entityCombatData[attackerId].targetId = targetId;
    std::cout << "[Combat] " << entityTypeToString(attackerId.type) << " '" << attackerId.name 
              << "' targeted " << entityTypeToString(targetId.type) << " '" << targetId.name << "'" << std::endl;
}

EntityId CombatSystem::getTarget(const EntityId& attackerId) const {
    auto it = m_entityCombatData.find(attackerId);
    return it != m_entityCombatData.end() ? it->second.targetId : EntityId();
}

void CombatSystem::clearTarget(const EntityId& attackerId) {
    auto it = m_entityCombatData.find(attackerId);
    if (it != m_entityCombatData.end()) {
        it->second.targetId = EntityId();
    }
}

void CombatSystem::startAutoAttack(const EntityId& attackerId) {
    auto& data = m_entityCombatData[attackerId];
    data.autoAttacking = true;
    data.autoAttackTimer = 0.0f;
}

void CombatSystem::stopAutoAttack(const EntityId& attackerId) {
    auto it = m_entityCombatData.find(attackerId);
    if (it != m_entityCombatData.end()) {
        it->second.autoAttacking = false;
    }
}

bool CombatSystem::processAutoAttack(const EntityId& attackerId, float deltaTime) {
    auto& data = m_entityCombatData[attackerId];
    
    if (!data.targetId.isValid()) {
        return false;
    }

    if (!isInRange(attackerId, data.targetId)) {
        return false;
    }

    data.autoAttackTimer += deltaTime;
    
    if (data.autoAttackTimer >= AUTO_ATTACK_SPEED) {
        data.autoAttackTimer = 0.0f;
        
        // TODO: Get weapon damage from player equipment
        float baseDamage = 50.0f;
        float damage = calculateDamage(attackerId, data.targetId, baseDamage, DamageType::PHYSICAL);
        
        applyDamage(data.targetId, damage, attackerId);
        enterCombat(attackerId);
        
        return true;
    }
    
    return false;
}

bool CombatSystem::useAbility(const EntityId& entityId, int abilitySlot) {
    auto& data = m_entityCombatData[entityId];
    
    // Check if ability is on cooldown
    if (!isAbilityReady(entityId, abilitySlot)) {
        return false;
    }

    if (!data.targetId.isValid()) {
        return false;
    }

    // TODO: Get ability data from player's learned abilities
    float abilityCooldown = 10.0f; // Default 10 second cooldown
    float abilityDamage = 150.0f;  // Default damage
    
    data.abilityCooldowns[abilitySlot] = abilityCooldown;
    
    float damage = calculateDamage(entityId, data.targetId, abilityDamage, DamageType::MAGICAL);
    applyDamage(data.targetId, damage, entityId);
    enterCombat(entityId);
    
    std::cout << "[Combat] " << entityTypeToString(entityId.type) << " '" << entityId.name 
              << "' used ability " << abilitySlot << std::endl;
    return true;
}

bool CombatSystem::isAbilityReady(const EntityId& entityId, int abilitySlot) const {
    auto it = m_entityCombatData.find(entityId);
    if (it == m_entityCombatData.end()) {
        return true;
    }
    
    auto cdIt = it->second.abilityCooldowns.find(abilitySlot);
    return cdIt == it->second.abilityCooldowns.end() || cdIt->second <= 0.0f;
}

float CombatSystem::getAbilityCooldown(const EntityId& entityId, int abilitySlot) const {
    auto it = m_entityCombatData.find(entityId);
    if (it == m_entityCombatData.end()) {
        return 0.0f;
    }
    
    auto cdIt = it->second.abilityCooldowns.find(abilitySlot);
    return cdIt != it->second.abilityCooldowns.end() ? std::max(0.0f, cdIt->second) : 0.0f;
}

float CombatSystem::calculateDamage(const EntityId& attackerId, const EntityId& targetId, 
                                   float baseDamage, DamageType type) {
    // TODO: Get attacker's stats from player/monster data
    float attackPower = 100.0f; // Placeholder
    
    // TODO: Get target's armor/resistance from player/monster data
    float targetDefense = 50.0f; // Placeholder
    
    float damage = baseDamage + (attackPower * 0.5f);
    
    // Apply defense reduction (diminishing returns formula)
    if (type == DamageType::PHYSICAL || type == DamageType::MAGICAL) {
        float damageReduction = targetDefense / (targetDefense + 1000.0f);
        damage *= (1.0f - damageReduction);
    }
    
    // Check for critical hit
    bool isCrit = rollCritical(CRIT_CHANCE_BASE);
    if (isCrit) {
        damage *= getCritMultiplier();
    }
    
    // Combat event logging
    CombatEvent event;
    event.attackerId = attackerId;
    event.targetId = targetId;
    event.damage = damage;
    event.damageType = type;
    event.isCritical = isCrit;
    event.timestamp = std::chrono::system_clock::now();
    
    const_cast<CombatSystem*>(this)->addCombatEvent(event);
    
    return damage;
}

void CombatSystem::applyDamage(const EntityId& targetId, float damage, const EntityId& sourceId) {
    // Check if target is invulnerable
    if (isInvulnerable(targetId)) {
        std::cout << "[Combat] " << entityTypeToString(targetId.type) << " '" << targetId.name 
                  << "' is invulnerable, damage ignored" << std::endl;
        return;
    }
    
    // TODO: Integrate with Player/Monster health system
    std::cout << "[Combat] " << entityTypeToString(sourceId.type) << " '" << sourceId.name 
              << "' dealt " << damage << " damage to " << entityTypeToString(targetId.type) 
              << " '" << targetId.name << "'" << std::endl;
    
    // Enter combat for both parties
    const_cast<CombatSystem*>(this)->enterCombat(targetId);
    const_cast<CombatSystem*>(this)->enterCombat(sourceId);
}

void CombatSystem::applyHealing(const EntityId& targetId, float amount, const EntityId& sourceId) {
    // TODO: Integrate with Player/Monster health system
    std::cout << "[Combat] " << entityTypeToString(sourceId.type) << " '" << sourceId.name 
              << "' healed " << entityTypeToString(targetId.type) << " '" << targetId.name 
              << "' for " << amount << std::endl;
}

void CombatSystem::handleDeath(const EntityId& entityId) {
    auto& data = m_entityCombatData[entityId];
    data.state = CombatState::DEAD;
    data.autoAttacking = false;
    data.targetId = EntityId();
    
    std::cout << "[Combat] " << entityTypeToString(entityId.type) << " '" << entityId.name << "' died" << std::endl;
    // TODO: Drop to ghost form, show respawn UI
}

void CombatSystem::respawn(const EntityId& entityId) {
    auto& data = m_entityCombatData[entityId];
    data.state = CombatState::OUT_OF_COMBAT;
    
    // TODO: Restore entity to spawn point with partial health/mana
    std::cout << "[Combat] " << entityTypeToString(entityId.type) << " '" << entityId.name << "' respawned" << std::endl;
}

void CombatSystem::awardExperience(const EntityId& playerId, int experience) {
    // Only players receive experience
    if (playerId.type != EntityType::PLAYER) {
        return;
    }
    // TODO: Integrate with character progression system
    std::cout << "[Combat] Player '" << playerId.name << "' gained " << experience << " XP" << std::endl;
}

void CombatSystem::distributeLoot(const EntityId& monsterId, const std::vector<EntityId>& playerIds) {
    // TODO: Generate loot from monster loot table
    // TODO: Distribute to players based on participation
    std::cout << "[Combat] Distributing loot from monster '" << monsterId.name << "' to " << playerIds.size() << " players" << std::endl;
}

void CombatSystem::addCombatEvent(const CombatEvent& event) {
    m_combatLog.push_back(event);
    
    // Keep log size manageable
    if (m_combatLog.size() > 1000) {
        m_combatLog.erase(m_combatLog.begin(), m_combatLog.begin() + 500);
    }
}

std::vector<CombatEvent> CombatSystem::getCombatLog(const EntityId& entityId, int maxEvents) const {
    std::vector<CombatEvent> entityLog;
    
    for (auto it = m_combatLog.rbegin(); it != m_combatLog.rend() && entityLog.size() < static_cast<size_t>(maxEvents); ++it) {
        if (it->attackerId == entityId || it->targetId == entityId) {
            entityLog.push_back(*it);
        }
    }
    
    return entityLog;
}

bool CombatSystem::isInRange(const EntityId& attackerId, const EntityId& targetId) const {
    // TODO: Get actual positions from game world
    // For now, assume always in range
    return true;
}

bool CombatSystem::hasLineOfSight(const EntityId& attackerId, const EntityId& targetId) const {
    // TODO: Implement raycasting for line of sight
    return true;
}

float CombatSystem::getCritMultiplier() const {
    return 2.0f; // 200% damage on crit
}

bool CombatSystem::rollCritical(float critChance) const {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<> dis(0.0, 1.0);
    
    return dis(gen) < critChance;
}

void CombatSystem::setInvulnerable(const EntityId& entityId, bool invulnerable) {
    if (invulnerable) {
        m_invulnerableEntities[entityId] = true;
        std::cout << "[Combat] " << entityTypeToString(entityId.type) << " '" << entityId.name 
                  << "' is now INVULNERABLE" << std::endl;
    } else {
        m_invulnerableEntities.erase(entityId);
        std::cout << "[Combat] " << entityTypeToString(entityId.type) << " '" << entityId.name 
                  << "' is now VULNERABLE" << std::endl;
    }
}

bool CombatSystem::isInvulnerable(const EntityId& entityId) const {
    auto it = m_invulnerableEntities.find(entityId);
    return it != m_invulnerableEntities.end() && it->second;
}

} // namespace clonemine
