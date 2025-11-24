#pragma once

#include <string>
#include <unordered_map>

namespace clonemine {

enum class PhysicalDamageType {
    PIERCING,   // Arrows, daggers, bites
    SLASHING,   // Swords, axes, claws
    BLUNT       // Maces, hammers, headbutts
};

enum class ElementalDamageType {
    FIRE,
    ICE,
    LIGHTNING,
    HOLY,
    SHADOW,
    NATURE,
    ARCANE,
    NONE
};

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
    float lightningResist = 0.0f;
    float holyResist = 0.0f;
    float shadowResist = 0.0f;
    float natureResist = 0.0f;
    float arcaneResist = 0.0f;
    
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
            case ElementalDamageType::LIGHTNING: return lightningResist;
            case ElementalDamageType::HOLY: return holyResist;
            case ElementalDamageType::SHADOW: return shadowResist;
            case ElementalDamageType::NATURE: return natureResist;
            case ElementalDamageType::ARCANE: return arcaneResist;
            case ElementalDamageType::NONE: return 0.0f;
        }
        return 0.0f;
    }
};

} // namespace clonemine
