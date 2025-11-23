#include "DamageCalculation.h"
#include "DamageTypes.h"
#include <algorithm>
#include <cmath>

namespace clonemine {

float DamageCalculation::calculateDamage(const DamageInstance& instance, const Target& target) {
    float totalDamage = 0.0f;
    
    for (const auto& component : instance.components) {
        float damage = component.amount;
        
        // Apply resistance
        float resistance = target.getResistance(component.type);
        resistance = std::clamp(resistance, -2.0f, 0.9f); // Cap at -200% to +90%
        
        float resistedDamage = damage * (1.0f - resistance);
        totalDamage += resistedDamage;
    }
    
    // Apply armor mitigation (physical damage only)
    if (instance.hasPhysicalDamage()) {
        float armorReduction = calculateArmorMitigation(target.armor);
        totalDamage *= (1.0f - armorReduction);
    }
    
    // Apply defensive stats (CON reduction capped at 20%)
    float conReduction = std::min(target.constitution * 0.002f, 0.2f);
    totalDamage *= (1.0f - conReduction);
    
    // Critical hits
    if (instance.isCritical) {
        totalDamage *= instance.criticalMultiplier;
    }
    
    // Apply damage modifiers
    totalDamage *= instance.damageModifier;
    
    return std::max(totalDamage, 1.0f); // Minimum 1 damage
}

float DamageCalculation::calculateArmorMitigation(float armorValue) {
    // Diminishing returns formula: armor / (armor + 100)
    // 100 armor = 50% reduction
    // 300 armor = 75% reduction
    // 900 armor = 90% reduction (soft cap)
    return armorValue / (armorValue + 100.0f);
}

DamageInstance DamageCalculation::createWeaponDamage(const Weapon& weapon, const Stats& attackerStats) {
    DamageInstance instance;
    
    float baseDamage = weapon.baseDamage;
    
    // Apply stat scaling based on weapon type
    if (weapon.scalesWith == StatType::STRENGTH) {
        baseDamage += attackerStats.strength * weapon.scalingFactor;
    } else if (weapon.scalesWith == StatType::DEXTERITY) {
        baseDamage += attackerStats.dexterity * weapon.scalingFactor;
    }
    
    // Add damage components based on weapon damage types
    for (const auto& [damageType, percentage] : weapon.damageTypes) {
        float damageAmount = baseDamage * percentage;
        instance.components.push_back({damageType, damageAmount});
    }
    
    // Check for critical hit
    float critChance = 0.05f + (attackerStats.dexterity * 0.001f); // Base 5% + DEX bonus
    if (rollCritical(critChance)) {
        instance.isCritical = true;
        instance.criticalMultiplier = 1.5f + (attackerStats.strength * 0.005f); // Base 1.5x + STR bonus
    }
    
    return instance;
}

DamageInstance DamageCalculation::createSpellDamage(const Spell& spell, const Stats& casterStats) {
    DamageInstance instance;
    
    float baseDamage = spell.baseDamage;
    
    // Apply stat scaling based on spell type
    if (spell.scalesWith == StatType::INTELLIGENCE) {
        baseDamage += casterStats.intelligence * spell.scalingFactor;
    } else if (spell.scalesWith == StatType::WISDOM) {
        baseDamage += casterStats.wisdom * spell.scalingFactor;
    } else if (spell.scalesWith == StatType::CHARISMA) {
        baseDamage += casterStats.charisma * spell.scalingFactor;
    }
    
    // Add damage components
    for (const auto& [damageType, percentage] : spell.damageTypes) {
        float damageAmount = baseDamage * percentage;
        instance.components.push_back({damageType, damageAmount});
    }
    
    // Spell crit chance (lower than physical)
    float critChance = 0.02f + (casterStats.intelligence * 0.0005f);
    if (rollCritical(critChance)) {
        instance.isCritical = true;
        instance.criticalMultiplier = 1.5f;
    }
    
    return instance;
}

bool DamageCalculation::rollCritical(float critChance) {
    float roll = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    return roll < critChance;
}

float Target::getResistance(DamageType type) const {
    float totalResistance = 0.0f;
    
    // Base armor type resistance
    if (armor) {
        auto it = armor->resistances.find(type);
        if (it != armor->resistances.end()) {
            totalResistance += it->second;
        }
    }
    
    // Monster-specific resistances
    for (const auto& [resistType, value] : resistances) {
        if (resistType == type) {
            totalResistance += value;
        }
    }
    
    // Temporary buffs/debuffs
    for (const auto& buff : activeBuffs) {
        if (buff.resistanceType == type) {
            totalResistance += buff.resistanceValue;
        }
    }
    
    return totalResistance;
}

bool DamageInstance::hasPhysicalDamage() const {
    for (const auto& component : components) {
        if (component.type == DamageType::PIERCING ||
            component.type == DamageType::SLASHING ||
            component.type == DamageType::BLUNT) {
            return true;
        }
    }
    return false;
}

} // namespace clonemine
