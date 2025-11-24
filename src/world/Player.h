#pragma once

#include <glm/glm.hpp>
#include "../combat/DamageTypes.h"
#include "../rpg/CharacterClass.h"
#include <string>
#include <vector>
#include <unordered_map>

namespace clonemine {

// Forward declarations
class Spell;
class Item;

// Player stats
struct PlayerStats {
    // D&D stats
    int strength = 10;
    int dexterity = 10;
    int constitution = 10;
    int intelligence = 10;
    int wisdom = 10;
    int charisma = 10;
    
    // Derived stats
    float getHealthMultiplier() const {
        return 1.0f + (constitution - 10) * 0.05f;
    }
    
    float getDamageMultiplier() const {
        // Varies by class (INT for casters, STR for fighters)
        return 1.0f;
    }
    
    float getDamageReduction() const {
        float reduction = (constitution - 10) * 0.02f;
        return std::min(reduction, 0.20f); // Cap at 20%
    }
};

// Equipment slots
enum class EquipmentSlot {
    MAIN_HAND,
    OFF_HAND,
    HELMET,
    SHOULDERS,
    CHEST,
    BELT,
    BRACERS,
    GLOVES,
    LEGS,
    BOOTS,
    NECKLACE,
    RING1,
    RING2
};

// Player class
class Player {
public:
    Player();
    ~Player() = default;
    
    // Update and movement
    void update(float deltaTime);
    void move(const glm::vec3& velocity);
    void jump();
    
    // Position and orientation
    const glm::vec3& getPosition() const { return m_position; }
    void setPosition(const glm::vec3& pos) { m_position = pos; }
    
    const glm::vec3& getVelocity() const { return m_velocity; }
    
    float getYaw() const { return m_yaw; }
    float getPitch() const { return m_pitch; }
    void setRotation(float yaw, float pitch);
    
    glm::vec3 getForward() const;
    glm::vec3 getRight() const;
    
    // Character progression
    void setClass(const std::string& className);
    const std::string& getClassName() const { return m_className; }
    
    int getLevel() const { return m_level; }
    void setLevel(int level);
    void addExperience(int xp);
    
    // Stats
    const PlayerStats& getStats() const { return m_stats; }
    PlayerStats& getStats() { return m_stats; }
    
    void addStatPoint(const std::string& stat, int points);
    
    // Health and resources
    float getHealth() const { return m_health; }
    float getMaxHealth() const { return m_maxHealth; }
    void setHealth(float health);
    void heal(float amount);
    void takeDamage(const DamageInfo& damage);
    
    float getResource() const { return m_resource; } // Mana/Energy/Focus
    float getMaxResource() const { return m_maxResource; }
    void setResource(float resource);
    void useResource(float amount);
    void regenerateResource(float amount);
    
    bool isAlive() const { return m_health > 0.0f; }
    bool isDead() const { return m_health <= 0.0f; }
    
    // Combat
    ResistanceProfile getResistances() const;
    bool canDodge() const;
    bool canParry() const;
    bool canBlock() const;
    
    // Equipment
    void equipItem(EquipmentSlot slot, Item* item);
    Item* getEquippedItem(EquipmentSlot slot) const;
    void unequipItem(EquipmentSlot slot);
    
    // Inventory
    bool addItem(Item* item);
    bool removeItem(Item* item);
    const std::vector<Item*>& getInventory() const { return m_inventory; }
    
    // Spells and abilities
    void learnSpell(Spell* spell);
    bool canCastSpell(Spell* spell) const;
    void castSpell(Spell* spell);
    const std::vector<Spell*>& getKnownSpells() const { return m_knownSpells; }
    
    // State
    bool isOnGround() const { return m_onGround; }
    bool isInWater() const { return m_inWater; }
    bool isFlying() const { return m_flying; }
    
private:
    // Transform
    glm::vec3 m_position{0.0f, 100.0f, 0.0f};
    glm::vec3 m_velocity{0.0f};
    float m_yaw = 0.0f;
    float m_pitch = 0.0f;
    
    // Character
    std::string m_className = "Fighter";
    int m_level = 1;
    int m_experience = 0;
    int m_experienceToNextLevel = 100;
    
    // Stats
    PlayerStats m_stats;
    
    // Health and resources
    float m_health = 100.0f;
    float m_maxHealth = 100.0f;
    float m_resource = 100.0f;      // Mana/Energy/Focus
    float m_maxResource = 100.0f;
    
    // Equipment
    std::unordered_map<EquipmentSlot, Item*> m_equipment;
    
    // Inventory
    std::vector<Item*> m_inventory;
    int m_maxInventorySlots = 30;
    
    // Spells
    std::vector<Spell*> m_knownSpells;
    
    // State
    bool m_onGround = false;
    bool m_inWater = false;
    bool m_flying = false;
    
    // Physics constants
    const float GRAVITY = 9.8f;
    const float WALK_SPEED = 4.3f;
    const float SPRINT_SPEED = 5.6f;
    const float JUMP_VELOCITY = 8.0f;
    
    // Resource regeneration
    float m_resourceRegenTimer = 0.0f;
    const float RESOURCE_REGEN_RATE = 5.0f; // per second (out of combat)
    
    // Helper functions
    void applyGravity(float deltaTime);
    void updateResourceRegeneration(float deltaTime);
    int calculateExperienceForLevel(int level) const;
};

} // namespace clonemine
