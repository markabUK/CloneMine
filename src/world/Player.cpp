#include "Player.h"
#include "../rpg/Spell.h"
#include "../combat/DamageCalculation.h"
#include <cmath>
#include <algorithm>

namespace clonemine {

Player::Player() {
    // Initialize default character
    m_maxHealth = 100.0f + m_stats.getHealthMultiplier() * 100.0f;
    m_health = m_maxHealth;
    m_maxResource = 100.0f;
    m_resource = m_maxResource;
}

void Player::update(float deltaTime) {
    // Apply gravity
    if (!m_onGround && !m_flying) {
        applyGravity(deltaTime);
    }
    
    // Update position
    m_position += m_velocity * deltaTime;
    
    // Resource regeneration
    updateResourceRegeneration(deltaTime);
    
    // Reset velocity if on ground
    if (m_onGround) {
        m_velocity.y = 0.0f;
    }
}

void Player::move(const glm::vec3& velocity) {
    m_velocity.x = velocity.x;
    m_velocity.z = velocity.z;
}

void Player::jump() {
    if (m_onGround) {
        m_velocity.y = JUMP_VELOCITY;
        m_onGround = false;
    }
}

void Player::setRotation(float yaw, float pitch) {
    m_yaw = yaw;
    m_pitch = std::clamp(pitch, -89.0f, 89.0f);
}

glm::vec3 Player::getForward() const {
    float yawRad = glm::radians(m_yaw);
    float pitchRad = glm::radians(m_pitch);
    
    return glm::normalize(glm::vec3(
        cos(pitchRad) * sin(yawRad),
        sin(pitchRad),
        cos(pitchRad) * cos(yawRad)
    ));
}

glm::vec3 Player::getRight() const {
    float yawRad = glm::radians(m_yaw);
    return glm::normalize(glm::vec3(
        cos(yawRad),
        0.0f,
        -sin(yawRad)
    ));
}

void Player::setClass(const std::string& className) {
    m_className = className;
    // TODO: Load class-specific stats and abilities
}

void Player::setLevel(int level) {
    if (level > m_level) {
        // int levelGain = level - m_level;  // TODO: Use this for stat point allocation
        m_level = level;
        
        // Award stat points (3 per level in D&D style)
        // Player can allocate these manually
        
        // Recalculate max health
        m_maxHealth = 100.0f + m_level * 10.0f * m_stats.getHealthMultiplier();
        m_health = m_maxHealth;
        
        // Recalculate max resource
        m_maxResource = 100.0f + m_level * 5.0f;
        m_resource = m_maxResource;
    }
}

void Player::addExperience(int xp) {
    m_experience += xp;
    
    // Level up if enough XP
    while (m_experience >= m_experienceToNextLevel) {
        m_experience -= m_experienceToNextLevel;
        setLevel(m_level + 1);
        m_experienceToNextLevel = calculateExperienceForLevel(m_level + 1);
    }
}

void Player::addStatPoint(const std::string& stat, int points) {
    if (stat == "strength") m_stats.strength += points;
    else if (stat == "dexterity") m_stats.dexterity += points;
    else if (stat == "constitution") m_stats.constitution += points;
    else if (stat == "intelligence") m_stats.intelligence += points;
    else if (stat == "wisdom") m_stats.wisdom += points;
    else if (stat == "charisma") m_stats.charisma += points;
    
    // Recalculate derived stats
    m_maxHealth = 100.0f + m_level * 10.0f * m_stats.getHealthMultiplier();
}

void Player::setHealth(float health) {
    m_health = std::clamp(health, 0.0f, m_maxHealth);
}

void Player::heal(float amount) {
    m_health = std::min(m_health + amount, m_maxHealth);
}

void Player::takeDamage(const DamageInfo& damage) {
    if (isDead()) return;
    
    // Get resistances from equipment
    ResistanceProfile resistances = getResistances();
    
    // Calculate final damage
    float finalDamage = DamageCalculation::calculateDamage(damage, resistances);
    
    // Apply damage reduction from CON (capped at 20%)
    finalDamage *= (1.0f - m_stats.getDamageReduction());
    
    // Apply damage
    m_health -= finalDamage;
    if (m_health < 0.0f) {
        m_health = 0.0f;
        // TODO: Handle death
    }
}

void Player::setResource(float resource) {
    m_resource = std::clamp(resource, 0.0f, m_maxResource);
}

void Player::useResource(float amount) {
    m_resource = std::max(m_resource - amount, 0.0f);
}

void Player::regenerateResource(float amount) {
    m_resource = std::min(m_resource + amount, m_maxResource);
}

ResistanceProfile Player::getResistances() const {
    ResistanceProfile profile;
    // TODO: Calculate from equipped armor
    // For now, return default
    return profile;
}

bool Player::canDodge() const {
    // Most classes can dodge
    return true;
}

bool Player::canParry() const {
    // Fighter, Ranger can parry
    return m_className == "Fighter" || m_className == "Ranger";
}

bool Player::canBlock() const {
    // Need shield equipped
    // Fighter, Priest can block
    return (m_className == "Fighter" || m_className == "Priest") &&
           m_equipment.count(EquipmentSlot::OFF_HAND) > 0;
}

void Player::equipItem(EquipmentSlot slot, Item* item) {
    // Unequip current item if any
    unequipItem(slot);
    
    // Equip new item
    m_equipment[slot] = item;
    
    // TODO: Apply item stats and bonuses
}

Item* Player::getEquippedItem(EquipmentSlot slot) const {
    auto it = m_equipment.find(slot);
    return (it != m_equipment.end()) ? it->second : nullptr;
}

void Player::unequipItem(EquipmentSlot slot) {
    auto it = m_equipment.find(slot);
    if (it != m_equipment.end()) {
        // TODO: Remove item stats and bonuses
        m_equipment.erase(it);
    }
}

bool Player::addItem(Item* item) {
    if (m_inventory.size() >= static_cast<size_t>(m_maxInventorySlots)) {
        return false; // Inventory full
    }
    
    m_inventory.push_back(item);
    return true;
}

bool Player::removeItem(Item* item) {
    auto it = std::find(m_inventory.begin(), m_inventory.end(), item);
    if (it != m_inventory.end()) {
        m_inventory.erase(it);
        return true;
    }
    return false;
}

void Player::learnSpell(Spell* spell) {
    // Check if already known
    if (std::find(m_knownSpells.begin(), m_knownSpells.end(), spell) == m_knownSpells.end()) {
        m_knownSpells.push_back(spell);
    }
}

bool Player::canCastSpell(Spell* spell) const {
    // Check if known
    if (std::find(m_knownSpells.begin(), m_knownSpells.end(), spell) == m_knownSpells.end()) {
        return false;
    }
    
    // Check resource cost
    // TODO: Get spell cost from spell
    float cost = 20.0f;
    return m_resource >= cost;
}

void Player::castSpell(Spell* spell) {
    if (!canCastSpell(spell)) return;
    
    // Use resource
    float cost = 20.0f; // TODO: Get from spell
    useResource(cost);
    
    // TODO: Apply spell effects
}

void Player::applyGravity(float deltaTime) {
    m_velocity.y -= GRAVITY * deltaTime;
}

void Player::updateResourceRegeneration(float deltaTime) {
    m_resourceRegenTimer += deltaTime;
    
    if (m_resourceRegenTimer >= 1.0f) {
        m_resourceRegenTimer = 0.0f;
        
        // Regenerate resource (out of combat rate)
        regenerateResource(RESOURCE_REGEN_RATE);
    }
}

int Player::calculateExperienceForLevel(int level) const {
    // Exponential XP curve
    return static_cast<int>(100 * std::pow(1.5, level - 1));
}

} // namespace clonemine
