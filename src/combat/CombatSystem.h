#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <chrono>
#include "../core/EntityId.h"

// Forward declarations
class Player;
class Monster;
class Ability;

namespace clonemine {

enum class CombatState {
    OUT_OF_COMBAT,
    IN_COMBAT,
    DEAD
};

enum class DamageType {
    PHYSICAL,
    MAGICAL,
    TRUE_DAMAGE
};

struct CombatEvent {
    EntityId attackerId;
    EntityId targetId;
    float damage;
    DamageType damageType;
    bool isCritical;
    std::chrono::system_clock::time_point timestamp;
};

class CombatSystem {
public:
    CombatSystem();
    ~CombatSystem();

    // Combat state management
    void update(float deltaTime);
    void enterCombat(const EntityId& entityId);
    void leaveCombat(const EntityId& entityId);
    bool isInCombat(const EntityId& entityId) const;

    // Targeting - now with EntityType to distinguish between player and monster with same name
    void setTarget(const EntityId& attackerId, const EntityId& targetId);
    EntityId getTarget(const EntityId& attackerId) const;
    void clearTarget(const EntityId& attackerId);

    // Auto-attack
    void startAutoAttack(const EntityId& attackerId);
    void stopAutoAttack(const EntityId& attackerId);
    bool processAutoAttack(const EntityId& attackerId, float deltaTime);

    // Abilities
    bool useAbility(const EntityId& entityId, int abilitySlot);
    bool isAbilityReady(const EntityId& entityId, int abilitySlot) const;
    float getAbilityCooldown(const EntityId& entityId, int abilitySlot) const;

    // Damage calculation - EntityId includes type to differentiate player/monster with same name
    float calculateDamage(const EntityId& attackerId, const EntityId& targetId, float baseDamage, DamageType type);
    void applyDamage(const EntityId& targetId, float damage, const EntityId& sourceId);
    void applyHealing(const EntityId& targetId, float amount, const EntityId& sourceId);

    // Death and respawn
    void handleDeath(const EntityId& entityId);
    void respawn(const EntityId& entityId);

    // Experience and loot
    void awardExperience(const EntityId& playerId, int experience);
    void distributeLoot(const EntityId& monsterId, const std::vector<EntityId>& playerIds);

    // Combat log
    void addCombatEvent(const CombatEvent& event);
    std::vector<CombatEvent> getCombatLog(const EntityId& entityId, int maxEvents = 50) const;

    // Invulnerability (for scripted scenes) - uses EntityId for exact entity matching
    void setInvulnerable(const EntityId& entityId, bool invulnerable);
    bool isInvulnerable(const EntityId& entityId) const;

private:
    struct EntityCombatData {
        EntityId targetId;
        CombatState state;
        float autoAttackTimer;
        float lastCombatTime;
        std::unordered_map<int, float> abilityCooldowns;
        bool autoAttacking;
    };

    std::unordered_map<EntityId, EntityCombatData> m_entityCombatData;
    std::vector<CombatEvent> m_combatLog;
    std::unordered_map<EntityId, bool> m_invulnerableEntities;

    // Constants
    static constexpr float AUTO_ATTACK_SPEED = 2.0f; // 2 seconds per attack
    static constexpr float COMBAT_TIMEOUT = 5.0f;    // 5 seconds out of combat
    static constexpr float MELEE_RANGE = 5.0f;
    static constexpr float RANGED_RANGE = 30.0f;
    static constexpr float CRIT_CHANCE_BASE = 0.05f; // 5% base crit

    // Helper methods
    bool isInRange(const EntityId& attackerId, const EntityId& targetId) const;
    bool hasLineOfSight(const EntityId& attackerId, const EntityId& targetId) const;
    float getCritMultiplier() const;
    bool rollCritical(float critChance) const;
};

} // namespace clonemine
