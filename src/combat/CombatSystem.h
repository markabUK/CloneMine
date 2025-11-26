#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <chrono>

// Forward declarations
class Player;
class Monster;
class Ability;

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
    std::string attackerId;
    std::string targetId;
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
    void enterCombat(const std::string& playerId);
    void leaveCombat(const std::string& playerId);
    bool isInCombat(const std::string& playerId) const;

    // Targeting
    void setTarget(const std::string& playerId, const std::string& targetId);
    std::string getTarget(const std::string& playerId) const;
    void clearTarget(const std::string& playerId);

    // Auto-attack
    void startAutoAttack(const std::string& playerId);
    void stopAutoAttack(const std::string& playerId);
    bool processAutoAttack(const std::string& playerId, float deltaTime);

    // Abilities
    bool useAbility(const std::string& playerId, int abilitySlot);
    bool isAbilityReady(const std::string& playerId, int abilitySlot) const;
    float getAbilityCooldown(const std::string& playerId, int abilitySlot) const;

    // Damage calculation
    float calculateDamage(const std::string& attackerId, const std::string& targetId, float baseDamage, DamageType type);
    void applyDamage(const std::string& targetId, float damage, const std::string& sourceId);
    void applyHealing(const std::string& targetId, float amount, const std::string& sourceId);

    // Death and respawn
    void handleDeath(const std::string& playerId);
    void respawn(const std::string& playerId);

    // Experience and loot
    void awardExperience(const std::string& playerId, int experience);
    void distributeLoot(const std::string& monsterId, const std::vector<std::string>& playerIds);

    // Combat log
    void addCombatEvent(const CombatEvent& event);
    std::vector<CombatEvent> getCombatLog(const std::string& playerId, int maxEvents = 50) const;

    // Invulnerability (for scripted scenes)
    void setInvulnerable(const std::string& entityId, bool invulnerable);
    bool isInvulnerable(const std::string& entityId) const;

private:
    struct PlayerCombatData {
        std::string targetId;
        CombatState state;
        float autoAttackTimer;
        float lastCombatTime;
        std::unordered_map<int, float> abilityCooldowns;
        bool autoAttacking;
    };

    std::unordered_map<std::string, PlayerCombatData> m_playerCombatData;
    std::vector<CombatEvent> m_combatLog;
    std::unordered_map<std::string, bool> m_invulnerableEntities;

    // Constants
    static constexpr float AUTO_ATTACK_SPEED = 2.0f; // 2 seconds per attack
    static constexpr float COMBAT_TIMEOUT = 5.0f;    // 5 seconds out of combat
    static constexpr float MELEE_RANGE = 5.0f;
    static constexpr float RANGED_RANGE = 30.0f;
    static constexpr float CRIT_CHANCE_BASE = 0.05f; // 5% base crit

    // Helper methods
    bool isInRange(const std::string& playerId, const std::string& targetId) const;
    bool hasLineOfSight(const std::string& playerId, const std::string& targetId) const;
    float getCritMultiplier() const;
    bool rollCritical(float critChance) const;
};
