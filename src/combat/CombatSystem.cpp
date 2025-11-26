#include "CombatSystem.h"
#include <random>
#include <cmath>
#include <iostream>

CombatSystem::CombatSystem() {
    std::cout << "[CombatSystem] Initialized PvE combat system" << std::endl;
}

CombatSystem::~CombatSystem() = default;

void CombatSystem::update(float deltaTime) {
    auto currentTime = std::chrono::system_clock::now();
    
    for (auto& [playerId, data] : m_playerCombatData) {
        // Update ability cooldowns
        for (auto& [slot, cooldown] : data.abilityCooldowns) {
            if (cooldown > 0.0f) {
                cooldown -= deltaTime;
            }
        }

        // Auto-attack processing
        if (data.autoAttacking && !data.targetId.empty()) {
            if (processAutoAttack(playerId, deltaTime)) {
                data.lastCombatTime = std::chrono::duration<float>(
                    currentTime.time_since_epoch()).count();
            }
        }

        // Combat timeout check
        if (data.state == CombatState::IN_COMBAT) {
            float timeSinceLastCombat = std::chrono::duration<float>(
                currentTime.time_since_epoch()).count() - data.lastCombatTime;
            
            if (timeSinceLastCombat >= COMBAT_TIMEOUT) {
                leaveCombat(playerId);
            }
        }
    }
}

void CombatSystem::enterCombat(const std::string& playerId) {
    auto& data = m_playerCombatData[playerId];
    if (data.state != CombatState::IN_COMBAT) {
        data.state = CombatState::IN_COMBAT;
        data.lastCombatTime = std::chrono::duration<float>(
            std::chrono::system_clock::now().time_since_epoch()).count();
        std::cout << "[Combat] Player " << playerId << " entered combat" << std::endl;
    }
}

void CombatSystem::leaveCombat(const std::string& playerId) {
    auto it = m_playerCombatData.find(playerId);
    if (it != m_playerCombatData.end()) {
        it->second.state = CombatState::OUT_OF_COMBAT;
        it->second.autoAttacking = false;
        std::cout << "[Combat] Player " << playerId << " left combat" << std::endl;
    }
}

bool CombatSystem::isInCombat(const std::string& playerId) const {
    auto it = m_playerCombatData.find(playerId);
    return it != m_playerCombatData.end() && it->second.state == CombatState::IN_COMBAT;
}

void CombatSystem::setTarget(const std::string& playerId, const std::string& targetId) {
    m_playerCombatData[playerId].targetId = targetId;
    std::cout << "[Combat] Player " << playerId << " targeted " << targetId << std::endl;
}

std::string CombatSystem::getTarget(const std::string& playerId) const {
    auto it = m_playerCombatData.find(playerId);
    return it != m_playerCombatData.end() ? it->second.targetId : "";
}

void CombatSystem::clearTarget(const std::string& playerId) {
    auto it = m_playerCombatData.find(playerId);
    if (it != m_playerCombatData.end()) {
        it->second.targetId.clear();
    }
}

void CombatSystem::startAutoAttack(const std::string& playerId) {
    auto& data = m_playerCombatData[playerId];
    data.autoAttacking = true;
    data.autoAttackTimer = 0.0f;
}

void CombatSystem::stopAutoAttack(const std::string& playerId) {
    auto it = m_playerCombatData.find(playerId);
    if (it != m_playerCombatData.end()) {
        it->second.autoAttacking = false;
    }
}

bool CombatSystem::processAutoAttack(const std::string& playerId, float deltaTime) {
    auto& data = m_playerCombatData[playerId];
    
    if (data.targetId.empty()) {
        return false;
    }

    if (!isInRange(playerId, data.targetId)) {
        return false;
    }

    data.autoAttackTimer += deltaTime;
    
    if (data.autoAttackTimer >= AUTO_ATTACK_SPEED) {
        data.autoAttackTimer = 0.0f;
        
        // TODO: Get weapon damage from player equipment
        float baseDamage = 50.0f;
        float damage = calculateDamage(playerId, data.targetId, baseDamage, DamageType::PHYSICAL);
        
        applyDamage(data.targetId, damage, playerId);
        enterCombat(playerId);
        
        return true;
    }
    
    return false;
}

bool CombatSystem::useAbility(const std::string& playerId, int abilitySlot) {
    auto& data = m_playerCombatData[playerId];
    
    // Check if ability is on cooldown
    if (!isAbilityReady(playerId, abilitySlot)) {
        return false;
    }

    if (data.targetId.empty()) {
        return false;
    }

    // TODO: Get ability data from player's learned abilities
    float abilityCooldown = 10.0f; // Default 10 second cooldown
    float abilityDamage = 150.0f;  // Default damage
    
    data.abilityCooldowns[abilitySlot] = abilityCooldown;
    
    float damage = calculateDamage(playerId, data.targetId, abilityDamage, DamageType::MAGICAL);
    applyDamage(data.targetId, damage, playerId);
    enterCombat(playerId);
    
    std::cout << "[Combat] Player " << playerId << " used ability " << abilitySlot << std::endl;
    return true;
}

bool CombatSystem::isAbilityReady(const std::string& playerId, int abilitySlot) const {
    auto it = m_playerCombatData.find(playerId);
    if (it == m_playerCombatData.end()) {
        return true;
    }
    
    auto cdIt = it->second.abilityCooldowns.find(abilitySlot);
    return cdIt == it->second.abilityCooldowns.end() || cdIt->second <= 0.0f;
}

float CombatSystem::getAbilityCooldown(const std::string& playerId, int abilitySlot) const {
    auto it = m_playerCombatData.find(playerId);
    if (it == m_playerCombatData.end()) {
        return 0.0f;
    }
    
    auto cdIt = it->second.abilityCooldowns.find(abilitySlot);
    return cdIt != it->second.abilityCooldowns.end() ? std::max(0.0f, cdIt->second) : 0.0f;
}

float CombatSystem::calculateDamage(const std::string& attackerId, const std::string& targetId, 
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

void CombatSystem::applyDamage(const std::string& targetId, float damage, const std::string& sourceId) {
    // TODO: Integrate with Player/Monster health system
    std::cout << "[Combat] " << sourceId << " dealt " << damage << " damage to " << targetId << std::endl;
    
    // Enter combat for both parties
    const_cast<CombatSystem*>(this)->enterCombat(targetId);
    const_cast<CombatSystem*>(this)->enterCombat(sourceId);
}

void CombatSystem::applyHealing(const std::string& targetId, float amount, const std::string& sourceId) {
    // TODO: Integrate with Player/Monster health system
    std::cout << "[Combat] " << sourceId << " healed " << targetId << " for " << amount << std::endl;
}

void CombatSystem::handleDeath(const std::string& playerId) {
    auto& data = m_playerCombatData[playerId];
    data.state = CombatState::DEAD;
    data.autoAttacking = false;
    data.targetId.clear();
    
    std::cout << "[Combat] Player " << playerId << " died" << std::endl;
    // TODO: Drop to ghost form, show respawn UI
}

void CombatSystem::respawn(const std::string& playerId) {
    auto& data = m_playerCombatData[playerId];
    data.state = CombatState::OUT_OF_COMBAT;
    
    // TODO: Restore player to spawn point with partial health/mana
    std::cout << "[Combat] Player " << playerId << " respawned" << std::endl;
}

void CombatSystem::awardExperience(const std::string& playerId, int experience) {
    // TODO: Integrate with character progression system
    std::cout << "[Combat] Player " << playerId << " gained " << experience << " XP" << std::endl;
}

void CombatSystem::distributeLoot(const std::string& monsterId, const std::vector<std::string>& playerIds) {
    // TODO: Generate loot from monster loot table
    // TODO: Distribute to players based on participation
    std::cout << "[Combat] Distributing loot from " << monsterId << " to " << playerIds.size() << " players" << std::endl;
}

void CombatSystem::addCombatEvent(const CombatEvent& event) {
    m_combatLog.push_back(event);
    
    // Keep log size manageable
    if (m_combatLog.size() > 1000) {
        m_combatLog.erase(m_combatLog.begin(), m_combatLog.begin() + 500);
    }
}

std::vector<CombatEvent> CombatSystem::getCombatLog(const std::string& playerId, int maxEvents) const {
    std::vector<CombatEvent> playerLog;
    
    for (auto it = m_combatLog.rbegin(); it != m_combatLog.rend() && playerLog.size() < static_cast<size_t>(maxEvents); ++it) {
        if (it->attackerId == playerId || it->targetId == playerId) {
            playerLog.push_back(*it);
        }
    }
    
    return playerLog;
}

bool CombatSystem::isInRange(const std::string& playerId, const std::string& targetId) const {
    // TODO: Get actual positions from game world
    // For now, assume always in range
    return true;
}

bool CombatSystem::hasLineOfSight(const std::string& playerId, const std::string& targetId) const {
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
