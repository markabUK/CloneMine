#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace clonemine {

enum class PhysicalDamageType {
    PIERCING,   // Arrows, daggers, bites
    SLASHING,   // Swords, axes, claws
    BLUNT       // Maces, hammers, headbutts
};

enum class ElementalDamageType {
    FIRE,           // Fire damage
    ICE,            // Ice/frost damage
    NATURE,         // Nature damage (can heal)
    ACID,           // Acid damage (corrosive)
    NECROTIC,       // Necrotic damage (life drain, undead)
    ARCANE,         // Pure arcane magic
    SHADOW,         // Shadow damage
    RADIANT,        // Radiant damage (healing, barriers, protection)
    HOLY,           // Holy damage (healing and attacking)
    LIGHTNING,      // Lightning damage (special nature damage)
    CONJURING,      // Summoning and conjuration magic
    PSIONIC,        // Mental/psychic damage
    WATER,          // Water damage (can attack and heal)
    NONE
};

// Magic capability flags for schools that can do multiple things
enum class MagicCapability {
    ATTACK = 1 << 0,       // Can deal damage
    HEAL = 1 << 1,         // Can heal
    BARRIER = 1 << 2,      // Can create barriers/protection
    SUMMON = 1 << 3,       // Can summon entities
    CONTROL = 1 << 4       // Can control/manipulate
};

inline int getMagicCapabilities(ElementalDamageType type) {
    switch (type) {
        case ElementalDamageType::FIRE:      return static_cast<int>(MagicCapability::ATTACK);
        case ElementalDamageType::ICE:       return static_cast<int>(MagicCapability::ATTACK) | static_cast<int>(MagicCapability::BARRIER);
        case ElementalDamageType::NATURE:    return static_cast<int>(MagicCapability::ATTACK) | static_cast<int>(MagicCapability::HEAL);
        case ElementalDamageType::ACID:      return static_cast<int>(MagicCapability::ATTACK);
        case ElementalDamageType::NECROTIC:  return static_cast<int>(MagicCapability::ATTACK);
        case ElementalDamageType::ARCANE:    return static_cast<int>(MagicCapability::ATTACK);
        case ElementalDamageType::SHADOW:    return static_cast<int>(MagicCapability::ATTACK);
        case ElementalDamageType::RADIANT:   return static_cast<int>(MagicCapability::HEAL) | static_cast<int>(MagicCapability::BARRIER);
        case ElementalDamageType::HOLY:      return static_cast<int>(MagicCapability::ATTACK) | static_cast<int>(MagicCapability::HEAL);
        case ElementalDamageType::LIGHTNING: return static_cast<int>(MagicCapability::ATTACK);  // Special nature
        case ElementalDamageType::CONJURING: return static_cast<int>(MagicCapability::SUMMON);
        case ElementalDamageType::PSIONIC:   return static_cast<int>(MagicCapability::ATTACK) | static_cast<int>(MagicCapability::CONTROL);
        case ElementalDamageType::WATER:     return static_cast<int>(MagicCapability::ATTACK) | static_cast<int>(MagicCapability::HEAL);
        case ElementalDamageType::NONE:      return 0;
    }
    return 0;
}

inline bool canHeal(ElementalDamageType type) {
    return (getMagicCapabilities(type) & static_cast<int>(MagicCapability::HEAL)) != 0;
}

inline bool canAttack(ElementalDamageType type) {
    return (getMagicCapabilities(type) & static_cast<int>(MagicCapability::ATTACK)) != 0;
}

inline bool canBarrier(ElementalDamageType type) {
    return (getMagicCapabilities(type) & static_cast<int>(MagicCapability::BARRIER)) != 0;
}

struct DamageComponent {
    PhysicalDamageType physicalType;
    ElementalDamageType elementalType;
    float percentage;  // 0.0 to 1.0
    
    DamageComponent(PhysicalDamageType phys, float pct)
        : physicalType(phys), elementalType(ElementalDamageType::NONE), percentage(pct) {}
    
    DamageComponent(ElementalDamageType elem, float pct)
        : physicalType(PhysicalDamageType::PIERCING), elementalType(elem), percentage(pct) {}
};

struct DamageInfo {
    float baseDamage;
    std::vector<DamageComponent> components;
    
    DamageInfo(float base) : baseDamage(base) {}
    
    void addComponent(const DamageComponent& comp) {
        components.push_back(comp);
    }
};

struct ResistanceProfile {
    // Physical resistances (-1.0 to 1.0, where 1.0 = immune, -1.0 = double damage)
    float piercingResist = 0.0f;
    float slashingResist = 0.0f;
    float bluntResist = 0.0f;
    
    // Elemental resistances
    float fireResist = 0.0f;
    float iceResist = 0.0f;
    float natureResist = 0.0f;
    float acidResist = 0.0f;
    float necroticResist = 0.0f;
    float arcaneResist = 0.0f;
    float shadowResist = 0.0f;
    float radiantResist = 0.0f;
    float holyResist = 0.0f;
    float lightningResist = 0.0f;
    float conjuringResist = 0.0f;
    float psionicResist = 0.0f;
    float waterResist = 0.0f;
    
    float getResistance(PhysicalDamageType type) const {
        switch(type) {
            case PhysicalDamageType::PIERCING: return piercingResist;
            case PhysicalDamageType::SLASHING: return slashingResist;
            case PhysicalDamageType::BLUNT: return bluntResist;
        }
        return 0.0f;
    }
    
    float getResistance(ElementalDamageType type) const {
        switch(type) {
            case ElementalDamageType::FIRE: return fireResist;
            case ElementalDamageType::ICE: return iceResist;
            case ElementalDamageType::NATURE: return natureResist;
            case ElementalDamageType::ACID: return acidResist;
            case ElementalDamageType::NECROTIC: return necroticResist;
            case ElementalDamageType::ARCANE: return arcaneResist;
            case ElementalDamageType::SHADOW: return shadowResist;
            case ElementalDamageType::RADIANT: return radiantResist;
            case ElementalDamageType::HOLY: return holyResist;
            case ElementalDamageType::LIGHTNING: return lightningResist;
            case ElementalDamageType::CONJURING: return conjuringResist;
            case ElementalDamageType::PSIONIC: return psionicResist;
            case ElementalDamageType::WATER: return waterResist;
            case ElementalDamageType::NONE: return 0.0f;
        }
        return 0.0f;
    }
};

} // namespace clonemine
