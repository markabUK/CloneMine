#pragma once

#include <string>
#include <unordered_map>
#include "../core/EntityId.h"

namespace clonemine {

// Weapon categories
enum class WeaponType {
    // Melee weapons
    SWORD,
    AXE,
    MACE,
    DAGGER,
    STAFF,       // Melee staff (can also cast)
    POLEARM,
    FIST,        // Unarmed or fist weapons
    
    // Ranged weapons
    BOW,         // Fires arrows (physical)
    CROSSBOW,    // Fires bolts (physical)
    GUN,         // Fires bullets (physical)
    WAND,        // Fires magic bolts (magical, depends on school)
    THROWING,    // Throwing weapons (physical)
    
    NONE
};

// Magic schools for wands and spells
enum class MagicSchool {
    FIRE,        // Fire bolt
    ICE,         // Ice/Frost bolt
    NATURE,      // Nature bolt (can heal)
    ACID,        // Acid bolt (corrosive)
    NECROTIC,    // Necrotic bolt (life drain)
    ARCANE,      // Arcane bolt (pure magic)
    SHADOW,      // Shadow bolt
    RADIANT,     // Radiant bolt (healing, barriers)
    HOLY,        // Holy bolt (healing and attacking)
    LIGHTNING,   // Lightning bolt (special nature damage)
    CONJURING,   // Conjuration magic (summoning)
    PSIONIC,     // Psionic bolt (mental damage)
    WATER,       // Water bolt (can attack and heal)
    NONE
};

// Damage types for combat
enum class WeaponDamageType {
    PHYSICAL,    // Swords, axes, bows, guns
    FIRE,        // Fire wands
    ICE,         // Ice/Frost wands
    NATURE,      // Nature wands
    ACID,        // Acid wands
    NECROTIC,    // Necrotic wands
    ARCANE,      // Arcane wands
    SHADOW,      // Shadow wands
    RADIANT,     // Radiant wands (protective)
    HOLY,        // Holy wands
    LIGHTNING,   // Lightning wands
    CONJURING,   // Conjuration wands
    PSIONIC,     // Psionic wands
    WATER        // Water wands
};

// Weapon data structure
struct Weapon {
    uint32_t id;
    std::string name;
    WeaponType type;
    MagicSchool magicSchool;     // Only for wands
    
    float minDamage;
    float maxDamage;
    float attackSpeed;           // Attacks per second
    float range;                 // Attack range (melee ~5, ranged ~30-40)
    
    // Projectile info for ranged
    std::string projectileType;  // "arrow", "bullet", "magic_bolt", etc.
    float projectileSpeed;       // How fast projectile travels
    
    // Requirements
    int levelRequired;
    int strengthRequired;
    int agilityRequired;
    int intellectRequired;
    
    // Helpers
    bool isRanged() const {
        return type == WeaponType::BOW || 
               type == WeaponType::CROSSBOW || 
               type == WeaponType::GUN || 
               type == WeaponType::WAND ||
               type == WeaponType::THROWING;
    }
    
    bool isMagic() const {
        return type == WeaponType::WAND;
    }
    
    bool isMelee() const {
        return !isRanged();
    }
    
    WeaponDamageType getDamageType() const {
        if (type == WeaponType::WAND) {
            switch (magicSchool) {
                case MagicSchool::FIRE: return WeaponDamageType::FIRE;
                case MagicSchool::ICE: return WeaponDamageType::ICE;
                case MagicSchool::NATURE: return WeaponDamageType::NATURE;
                case MagicSchool::ACID: return WeaponDamageType::ACID;
                case MagicSchool::NECROTIC: return WeaponDamageType::NECROTIC;
                case MagicSchool::ARCANE: return WeaponDamageType::ARCANE;
                case MagicSchool::SHADOW: return WeaponDamageType::SHADOW;
                case MagicSchool::RADIANT: return WeaponDamageType::RADIANT;
                case MagicSchool::HOLY: return WeaponDamageType::HOLY;
                case MagicSchool::LIGHTNING: return WeaponDamageType::LIGHTNING;
                case MagicSchool::CONJURING: return WeaponDamageType::CONJURING;
                case MagicSchool::PSIONIC: return WeaponDamageType::PSIONIC;
                case MagicSchool::WATER: return WeaponDamageType::WATER;
                default: return WeaponDamageType::ARCANE;
            }
        }
        return WeaponDamageType::PHYSICAL;
    }
};

// Weapon system for managing equipped weapons
class WeaponSystem {
public:
    WeaponSystem();
    ~WeaponSystem() = default;
    
    // Weapon management
    void equipWeapon(const EntityId& entityId, const Weapon& weapon);
    void unequipWeapon(const EntityId& entityId);
    const Weapon* getEquippedWeapon(const EntityId& entityId) const;
    
    // Combat calculations
    float getAttackRange(const EntityId& entityId) const;
    float getAttackSpeed(const EntityId& entityId) const;
    float rollWeaponDamage(const EntityId& entityId) const;
    WeaponDamageType getDamageType(const EntityId& entityId) const;
    bool isRangedWeapon(const EntityId& entityId) const;
    bool isMagicWeapon(const EntityId& entityId) const;
    
    // Projectile info
    std::string getProjectileType(const EntityId& entityId) const;
    float getProjectileSpeed(const EntityId& entityId) const;
    MagicSchool getMagicSchool(const EntityId& entityId) const;
    
    // Weapon templates
    static Weapon createSword(uint32_t id, const std::string& name, float minDmg, float maxDmg, int level);
    static Weapon createAxe(uint32_t id, const std::string& name, float minDmg, float maxDmg, int level);
    static Weapon createBow(uint32_t id, const std::string& name, float minDmg, float maxDmg, int level);
    static Weapon createGun(uint32_t id, const std::string& name, float minDmg, float maxDmg, int level);
    static Weapon createWand(uint32_t id, const std::string& name, float minDmg, float maxDmg, int level, MagicSchool school);
    
private:
    std::unordered_map<EntityId, Weapon> m_equippedWeapons;
    
    // Default unarmed weapon
    static Weapon s_unarmedWeapon;
};

// Helper functions
inline const char* weaponTypeToString(WeaponType type) {
    switch (type) {
        case WeaponType::SWORD: return "Sword";
        case WeaponType::AXE: return "Axe";
        case WeaponType::MACE: return "Mace";
        case WeaponType::DAGGER: return "Dagger";
        case WeaponType::STAFF: return "Staff";
        case WeaponType::POLEARM: return "Polearm";
        case WeaponType::FIST: return "Fist Weapon";
        case WeaponType::BOW: return "Bow";
        case WeaponType::CROSSBOW: return "Crossbow";
        case WeaponType::GUN: return "Gun";
        case WeaponType::WAND: return "Wand";
        case WeaponType::THROWING: return "Throwing";
        case WeaponType::NONE: return "None";
        default: return "Unknown";
    }
}

inline const char* magicSchoolToString(MagicSchool school) {
    switch (school) {
        case MagicSchool::FIRE: return "Fire";
        case MagicSchool::ICE: return "Ice";
        case MagicSchool::NATURE: return "Nature";
        case MagicSchool::ACID: return "Acid";
        case MagicSchool::NECROTIC: return "Necrotic";
        case MagicSchool::ARCANE: return "Arcane";
        case MagicSchool::SHADOW: return "Shadow";
        case MagicSchool::RADIANT: return "Radiant";
        case MagicSchool::HOLY: return "Holy";
        case MagicSchool::LIGHTNING: return "Lightning";
        case MagicSchool::CONJURING: return "Conjuring";
        case MagicSchool::PSIONIC: return "Psionic";
        case MagicSchool::WATER: return "Water";
        case MagicSchool::NONE: return "None";
        default: return "Unknown";
    }
}

inline const char* weaponDamageTypeToString(WeaponDamageType type) {
    switch (type) {
        case WeaponDamageType::PHYSICAL: return "Physical";
        case WeaponDamageType::FIRE: return "Fire";
        case WeaponDamageType::ICE: return "Ice";
        case WeaponDamageType::NATURE: return "Nature";
        case WeaponDamageType::ACID: return "Acid";
        case WeaponDamageType::NECROTIC: return "Necrotic";
        case WeaponDamageType::ARCANE: return "Arcane";
        case WeaponDamageType::SHADOW: return "Shadow";
        case WeaponDamageType::RADIANT: return "Radiant";
        case WeaponDamageType::HOLY: return "Holy";
        case WeaponDamageType::LIGHTNING: return "Lightning";
        case WeaponDamageType::CONJURING: return "Conjuring";
        case WeaponDamageType::PSIONIC: return "Psionic";
        case WeaponDamageType::WATER: return "Water";
        default: return "Unknown";
    }
}

} // namespace clonemine
