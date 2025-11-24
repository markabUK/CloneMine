#pragma once

#include <string>
#include <vector>
#include <map>
#include <cstdint>
#include <glm/glm.hpp>

namespace clonemine {
namespace character {

// Item in inventory or equipped
struct ItemData {
    uint32_t itemId;
    std::string itemName;
    uint32_t quantity;
    uint32_t durability; // For equipment
    std::string itemType; // weapon, armor, consumable, material, etc.
};

// Equipment slots
struct EquipmentData {
    ItemData head;
    ItemData chest;
    ItemData legs;
    ItemData feet;
    ItemData mainHand;
    ItemData offHand;
    ItemData ring1;
    ItemData ring2;
    ItemData necklace;
    ItemData back;
    
    bool headEquipped{false};
    bool chestEquipped{false};
    bool legsEquipped{false};
    bool feetEquipped{false};
    bool mainHandEquipped{false};
    bool offHandEquipped{false};
    bool ring1Equipped{false};
    bool ring2Equipped{false};
    bool necklaceEquipped{false};
    bool backEquipped{false};
};

// Ability/Spell data
struct AbilityData {
    uint32_t abilityId;
    std::string abilityName;
    uint32_t level; // Ability level (can be upgraded)
    uint32_t cooldownRemaining; // In seconds
    bool unlocked{true};
};

// Quest progress
struct QuestProgress {
    uint32_t questId;
    std::string questName;
    bool completed{false};
    std::vector<uint32_t> objectiveProgress;
};

// Character data - complete state
struct CharacterData {
    uint32_t characterId;
    std::string name;
    std::string className;
    uint32_t level;
    uint32_t experience;
    uint32_t experienceToNextLevel;
    
    // Position and orientation
    glm::vec3 position;
    float yaw;
    float pitch;
    
    // Resources
    float health;
    float maxHealth;
    float resource; // Mana/energy/rage depending on class
    float maxResource;
    
    // Stats
    uint32_t strength;
    uint32_t dexterity;
    uint32_t constitution;
    uint32_t intelligence;
    uint32_t wisdom;
    uint32_t charisma;
    
    // Derived stats
    uint32_t armor;
    uint32_t magicResist;
    float critChance;
    float critDamage;
    float movementSpeed;
    
    // Currency
    uint32_t gold;
    uint32_t gems; // Premium currency
    
    // Inventory (up to max inventory size)
    std::vector<ItemData> inventory;
    uint32_t maxInventorySize{40};
    
    // Equipment
    EquipmentData equipment;
    
    // Abilities and spells
    std::vector<AbilityData> abilities;
    std::vector<AbilityData> spells;
    
    // Skill points
    uint32_t availableSkillPoints;
    uint32_t availableTalentPoints;
    
    // Quests
    std::vector<QuestProgress> activeQuests;
    std::vector<uint32_t> completedQuestIds;
    
    // Achievements
    std::vector<uint32_t> unlockedAchievements;
    
    // Faction reputation
    std::map<std::string, int32_t> factionReputation; // faction name -> reputation (-1000 to 1000)
    
    // Timestamps
    uint64_t createdTimestamp;
    uint64_t lastPlayedTimestamp;
    uint64_t lastSavedTimestamp;
    
    // Play time (in seconds)
    uint64_t totalPlayTime;
    
    // PvP stats
    uint32_t playerKills;
    uint32_t deaths;
    uint32_t pvpRating;
    
    // World state
    std::string currentZone;
    std::string lastCity; // For respawn/recall
};

struct CharacterSlot {
    bool occupied{false};
    CharacterData character;
};

struct Account {
    uint32_t accountId;
    std::string username;
    std::vector<CharacterSlot> characterSlots;
    uint32_t maxCharacters{5}; // Configurable limit
    
    Account() {
        characterSlots.resize(maxCharacters);
    }
    
    [[nodiscard]] bool hasAvailableSlot() const {
        for (const auto& slot : characterSlots) {
            if (!slot.occupied) {
                return true;
            }
        }
        return false;
    }
    
    [[nodiscard]] size_t getCharacterCount() const {
        size_t count = 0;
        for (const auto& slot : characterSlots) {
            if (slot.occupied) {
                count++;
            }
        }
        return count;
    }
    
    [[nodiscard]] int findAvailableSlot() const {
        for (size_t i = 0; i < characterSlots.size(); ++i) {
            if (!characterSlots[i].occupied) {
                return static_cast<int>(i);
            }
        }
        return -1;
    }
};

} // namespace character
} // namespace clonemine
