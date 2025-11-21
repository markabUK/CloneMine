#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

namespace RPG {

struct Stats {
    int strength = 10;
    int dexterity = 10;
    int constitution = 10;
    int intelligence = 10;
    int wisdom = 10;
    int charisma = 10;
};

struct ResourcePool {
    std::string name;
    int maxValue;
    int currentValue;
    float combatRegen;
    float outOfCombatRegen;
};

struct DamageScaling {
    std::string stat;           // STR, DEX, INT, etc.
    std::string damageType;     // physicalDamage, spellDamage, etc.
    float scalingFactor;        // Percentage per stat point
};

struct DefensiveMechanics {
    bool canDodge = false;
    bool canParry = false;
    bool canBlock = false;      // Requires shield
};

class CharacterClass {
public:
    CharacterClass() = default;
    
    // Getters
    const std::string& getName() const { return name; }
    const std::string& getPrimaryStat() const { return primaryStat; }
    const Stats& getBaseStats() const { return baseStats; }
    const ResourcePool& getResourcePool() const { return resourcePool; }
    const std::vector<std::string>& getEquipmentRestrictions() const { return equipmentRestrictions; }
    const std::vector<DamageScaling>& getDamageScaling() const { return damageScaling; }
    const DefensiveMechanics& getDefensiveMechanics() const { return defensiveMechanics; }
    
    // Setters
    void setName(const std::string& n) { name = n; }
    void setPrimaryStat(const std::string& stat) { primaryStat = stat; }
    void setBaseStats(const Stats& stats) { baseStats = stats; }
    void setResourcePool(const ResourcePool& pool) { resourcePool = pool; }
    void addEquipmentRestriction(const std::string& restriction) { 
        equipmentRestrictions.push_back(restriction); 
    }
    void addDamageScaling(const DamageScaling& scaling) {
        damageScaling.push_back(scaling);
    }
    void setDefensiveMechanics(const DefensiveMechanics& mechanics) {
        defensiveMechanics = mechanics;
    }
    
    // Utility
    bool canUseEquipment(const std::string& equipmentType) const;
    float calculateDamageMultiplier(const std::string& damageType, const Stats& currentStats) const;
    
private:
    std::string name;
    std::string primaryStat;
    Stats baseStats;
    ResourcePool resourcePool;
    std::vector<std::string> equipmentRestrictions;
    std::vector<DamageScaling> damageScaling;
    DefensiveMechanics defensiveMechanics;
};

} // namespace RPG
