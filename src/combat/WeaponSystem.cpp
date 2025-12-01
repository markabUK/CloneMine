#include "WeaponSystem.h"
#include <random>
#include <iostream>

namespace clonemine {

// Default unarmed weapon (fists)
Weapon WeaponSystem::s_unarmedWeapon = {
    0,                      // id
    "Unarmed",              // name
    WeaponType::FIST,       // type
    MagicSchool::NONE,      // magicSchool
    1.0f,                   // minDamage
    3.0f,                   // maxDamage
    1.5f,                   // attackSpeed (attacks per second)
    5.0f,                   // range (melee)
    "",                     // projectileType
    0.0f,                   // projectileSpeed
    1,                      // levelRequired
    0,                      // strengthRequired
    0,                      // agilityRequired
    0                       // intellectRequired
};

WeaponSystem::WeaponSystem() {
    std::cout << "[WeaponSystem] Initialized with ranged weapon support (wands, bows, guns)" << std::endl;
}

void WeaponSystem::equipWeapon(const EntityId& entityId, const Weapon& weapon) {
    m_equippedWeapons[entityId] = weapon;
    
    if (weapon.isRanged()) {
        if (weapon.isMagic()) {
            std::cout << "[Weapon] " << entityTypeToString(entityId.type) << " '" << entityId.name 
                      << "' equipped " << weapon.name << " (" << weaponTypeToString(weapon.type) 
                      << ", " << magicSchoolToString(weapon.magicSchool) << " magic bolt)" << std::endl;
        } else {
            std::cout << "[Weapon] " << entityTypeToString(entityId.type) << " '" << entityId.name 
                      << "' equipped " << weapon.name << " (" << weaponTypeToString(weapon.type) 
                      << ", ranged " << weapon.range << " units)" << std::endl;
        }
    } else {
        std::cout << "[Weapon] " << entityTypeToString(entityId.type) << " '" << entityId.name 
                  << "' equipped " << weapon.name << " (" << weaponTypeToString(weapon.type) 
                  << ", melee)" << std::endl;
    }
}

void WeaponSystem::unequipWeapon(const EntityId& entityId) {
    auto it = m_equippedWeapons.find(entityId);
    if (it != m_equippedWeapons.end()) {
        std::cout << "[Weapon] " << entityTypeToString(entityId.type) << " '" << entityId.name 
                  << "' unequipped " << it->second.name << std::endl;
        m_equippedWeapons.erase(it);
    }
}

const Weapon* WeaponSystem::getEquippedWeapon(const EntityId& entityId) const {
    auto it = m_equippedWeapons.find(entityId);
    if (it != m_equippedWeapons.end()) {
        return &it->second;
    }
    return &s_unarmedWeapon;
}

float WeaponSystem::getAttackRange(const EntityId& entityId) const {
    const Weapon* weapon = getEquippedWeapon(entityId);
    return weapon->range;
}

float WeaponSystem::getAttackSpeed(const EntityId& entityId) const {
    const Weapon* weapon = getEquippedWeapon(entityId);
    return weapon->attackSpeed;
}

float WeaponSystem::rollWeaponDamage(const EntityId& entityId) const {
    const Weapon* weapon = getEquippedWeapon(entityId);
    
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(weapon->minDamage, weapon->maxDamage);
    
    return dis(gen);
}

WeaponDamageType WeaponSystem::getDamageType(const EntityId& entityId) const {
    const Weapon* weapon = getEquippedWeapon(entityId);
    return weapon->getDamageType();
}

bool WeaponSystem::isRangedWeapon(const EntityId& entityId) const {
    const Weapon* weapon = getEquippedWeapon(entityId);
    return weapon->isRanged();
}

bool WeaponSystem::isMagicWeapon(const EntityId& entityId) const {
    const Weapon* weapon = getEquippedWeapon(entityId);
    return weapon->isMagic();
}

std::string WeaponSystem::getProjectileType(const EntityId& entityId) const {
    const Weapon* weapon = getEquippedWeapon(entityId);
    
    if (!weapon->isRanged()) {
        return "";
    }
    
    if (!weapon->projectileType.empty()) {
        return weapon->projectileType;
    }
    
    // Default projectile types
    switch (weapon->type) {
        case WeaponType::BOW:
        case WeaponType::CROSSBOW:
            return "arrow";
        case WeaponType::GUN:
            return "bullet";
        case WeaponType::WAND:
            switch (weapon->magicSchool) {
                case MagicSchool::FIRE: return "fire_bolt";
                case MagicSchool::FROST: return "frost_bolt";
                case MagicSchool::ARCANE: return "arcane_bolt";
                case MagicSchool::NATURE: return "nature_bolt";
                case MagicSchool::SHADOW: return "shadow_bolt";
                case MagicSchool::HOLY: return "holy_bolt";
                case MagicSchool::LIGHTNING: return "lightning_bolt";
                default: return "magic_bolt";
            }
        case WeaponType::THROWING:
            return "throwing_knife";
        default:
            return "";
    }
}

float WeaponSystem::getProjectileSpeed(const EntityId& entityId) const {
    const Weapon* weapon = getEquippedWeapon(entityId);
    
    if (weapon->projectileSpeed > 0.0f) {
        return weapon->projectileSpeed;
    }
    
    // Default projectile speeds
    switch (weapon->type) {
        case WeaponType::BOW: return 50.0f;
        case WeaponType::CROSSBOW: return 60.0f;
        case WeaponType::GUN: return 100.0f;
        case WeaponType::WAND: return 40.0f;
        case WeaponType::THROWING: return 35.0f;
        default: return 0.0f;
    }
}

MagicSchool WeaponSystem::getMagicSchool(const EntityId& entityId) const {
    const Weapon* weapon = getEquippedWeapon(entityId);
    return weapon->magicSchool;
}

// Factory methods for creating weapons
Weapon WeaponSystem::createSword(uint32_t id, const std::string& name, float minDmg, float maxDmg, int level) {
    Weapon weapon;
    weapon.id = id;
    weapon.name = name;
    weapon.type = WeaponType::SWORD;
    weapon.magicSchool = MagicSchool::NONE;
    weapon.minDamage = minDmg;
    weapon.maxDamage = maxDmg;
    weapon.attackSpeed = 2.0f;  // 2 attacks per second
    weapon.range = 5.0f;        // Melee range
    weapon.projectileType = "";
    weapon.projectileSpeed = 0.0f;
    weapon.levelRequired = level;
    weapon.strengthRequired = level * 2;
    weapon.agilityRequired = 0;
    weapon.intellectRequired = 0;
    return weapon;
}

Weapon WeaponSystem::createAxe(uint32_t id, const std::string& name, float minDmg, float maxDmg, int level) {
    Weapon weapon;
    weapon.id = id;
    weapon.name = name;
    weapon.type = WeaponType::AXE;
    weapon.magicSchool = MagicSchool::NONE;
    weapon.minDamage = minDmg;
    weapon.maxDamage = maxDmg;
    weapon.attackSpeed = 1.5f;  // Slower but harder hitting
    weapon.range = 5.0f;        // Melee range
    weapon.projectileType = "";
    weapon.projectileSpeed = 0.0f;
    weapon.levelRequired = level;
    weapon.strengthRequired = level * 3;
    weapon.agilityRequired = 0;
    weapon.intellectRequired = 0;
    return weapon;
}

Weapon WeaponSystem::createBow(uint32_t id, const std::string& name, float minDmg, float maxDmg, int level) {
    Weapon weapon;
    weapon.id = id;
    weapon.name = name;
    weapon.type = WeaponType::BOW;
    weapon.magicSchool = MagicSchool::NONE;
    weapon.minDamage = minDmg;
    weapon.maxDamage = maxDmg;
    weapon.attackSpeed = 1.5f;  // 1.5 attacks per second
    weapon.range = 35.0f;       // Long range
    weapon.projectileType = "arrow";
    weapon.projectileSpeed = 50.0f;
    weapon.levelRequired = level;
    weapon.strengthRequired = level;
    weapon.agilityRequired = level * 2;
    weapon.intellectRequired = 0;
    return weapon;
}

Weapon WeaponSystem::createGun(uint32_t id, const std::string& name, float minDmg, float maxDmg, int level) {
    Weapon weapon;
    weapon.id = id;
    weapon.name = name;
    weapon.type = WeaponType::GUN;
    weapon.magicSchool = MagicSchool::NONE;
    weapon.minDamage = minDmg;
    weapon.maxDamage = maxDmg;
    weapon.attackSpeed = 1.0f;  // Slower but high damage
    weapon.range = 40.0f;       // Very long range
    weapon.projectileType = "bullet";
    weapon.projectileSpeed = 100.0f;
    weapon.levelRequired = level;
    weapon.strengthRequired = 0;
    weapon.agilityRequired = level * 2;
    weapon.intellectRequired = 0;
    return weapon;
}

Weapon WeaponSystem::createWand(uint32_t id, const std::string& name, float minDmg, float maxDmg, int level, MagicSchool school) {
    Weapon weapon;
    weapon.id = id;
    weapon.name = name;
    weapon.type = WeaponType::WAND;
    weapon.magicSchool = school;
    weapon.minDamage = minDmg;
    weapon.maxDamage = maxDmg;
    weapon.attackSpeed = 1.8f;  // Fast attack speed
    weapon.range = 30.0f;       // Long range
    
    // Set projectile type based on magic school
    switch (school) {
        case MagicSchool::FIRE:
            weapon.projectileType = "fire_bolt";
            break;
        case MagicSchool::FROST:
            weapon.projectileType = "frost_bolt";
            break;
        case MagicSchool::ARCANE:
            weapon.projectileType = "arcane_bolt";
            break;
        case MagicSchool::NATURE:
            weapon.projectileType = "nature_bolt";
            break;
        case MagicSchool::SHADOW:
            weapon.projectileType = "shadow_bolt";
            break;
        case MagicSchool::HOLY:
            weapon.projectileType = "holy_bolt";
            break;
        case MagicSchool::LIGHTNING:
            weapon.projectileType = "lightning_bolt";
            break;
        default:
            weapon.projectileType = "magic_bolt";
            break;
    }
    
    weapon.projectileSpeed = 40.0f;
    weapon.levelRequired = level;
    weapon.strengthRequired = 0;
    weapon.agilityRequired = 0;
    weapon.intellectRequired = level * 3;
    return weapon;
}

} // namespace clonemine
