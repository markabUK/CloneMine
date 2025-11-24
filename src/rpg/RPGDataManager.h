#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <vector>
#include <optional>
#include "../plugin/LuaSandbox.h"

namespace CloneMine::RPG {

/**
 * @brief Comprehensive RPG data manager that loads and manages all Lua-based game data
 * 
 * This class provides a unified interface for loading and accessing:
 * - Character classes
 * - Spells (player and monster)
 * - Abilities (player and monster)
 * - Monsters and quests
 * - Equipment
 * - Items
 * - Random encounters
 */
class RPGDataManager {
public:
    /**
 * @brief Class statistics and configuration
     */
    struct ClassStats {
        std::string name;
        std::string primaryStat;
        int baseHealth;
        int baseResource;
        std::string resourceType;
        std::vector<std::string> allowedWeapons;
        std::vector<std::string> allowedArmor;
        std::unordered_map<std::string, double> damageScaling;
    };

    /**
     * @brief Spell definition
     */
    struct Spell {
        std::string name;
        std::string school;
        int baseDamage;
        int resourceCost;
        double castTime;
        double cooldown;
        std::vector<std::string> classes;
        std::unordered_map<std::string, int> tint; // RGB color
    };

    /**
     * @brief Ability definition
     */
    struct Ability {
        std::string name;
        std::string type;
        int baseDamage;
        int resourceCost;
        double cooldown;
        std::vector<std::string> classes;
    };

    /**
     * @brief Monster definition
     */
    struct Monster {
        std::string name;
        int level;
        int health;
        int damage;
        std::string difficulty;
        std::vector<std::string> abilities;
        std::vector<std::string> loot;
    };

    /**
     * @brief Pet definition
     */
    struct Pet {
        std::string name;
        std::string type;
        int baseHealth;
        int baseDamage;
        double damageScaling;
        std::vector<std::string> abilities;
    };

    explicit RPGDataManager(Plugin::LuaSandbox& sandbox);
    ~RPGDataManager() = default;

    // Delete copy/move constructors (singleton pattern)
    RPGDataManager(const RPGDataManager&) = delete;
    RPGDataManager& operator=(const RPGDataManager&) = delete;
    RPGDataManager(RPGDataManager&&) = delete;
    RPGDataManager& operator=(RPGDataManager&&) = delete;

    /**
     * @brief Load all RPG data from Lua files
     * @param dataPath Path to the data directory
     * @return true if all data loaded successfully
     */
    bool loadAllData(const std::string& dataPath);

    /**
     * @brief Load character classes
     */
    bool loadClasses(const std::string& classPath);

    /**
     * @brief Load spells
     */
    bool loadSpells(const std::string& spellPath);

    /**
     * @brief Load abilities
     */
    bool loadAbilities(const std::string& abilityPath);

    /**
     * @brief Load monsters
     */
    bool loadMonsters(const std::string& monsterPath);

    /**
     * @brief Load equipment data
     */
    bool loadEquipment(const std::string& equipPath);

    /**
     * @brief Get class stats by name
     */
    std::optional<ClassStats> getClass(const std::string& className) const;

    /**
     * @brief Get spell by name
     */
    std::optional<Spell> getSpell(const std::string& spellName) const;

    /**
     * @brief Get ability by name
     */
    std::optional<Ability> getAbility(const std::string& abilityName) const;

    /**
     * @brief Get monster by name
     */
    std::optional<Monster> getMonster(const std::string& monsterName) const;

    /**
     * @brief Get all spells for a specific class
     */
    std::vector<Spell> getSpellsForClass(const std::string& className) const;

    /**
     * @brief Get all abilities for a specific class
     */
    std::vector<Ability> getAbilitiesForClass(const std::string& className) const;

    /**
     * @brief Get pets for a class
     */
    std::vector<Pet> getPetsForClass(const std::string& className) const;

    /**
     * @brief Validate all loaded data
     * @return Vector of validation error messages (empty if all valid)
     */
    std::vector<std::string> validate() const;

    /**
     * @brief Get statistics about loaded data
     */
    struct DataStats {
        size_t classCount;
        size_t spellCount;
        size_t abilityCount;
        size_t monsterCount;
        size_t petCount;
    };
    DataStats getDataStats() const;

private:
    Plugin::LuaSandbox& m_sandbox;
    
    // Data storage
    std::unordered_map<std::string, ClassStats> m_classes;
    std::unordered_map<std::string, Spell> m_spells;
    std::unordered_map<std::string, Ability> m_abilities;
    std::unordered_map<std::string, Monster> m_monsters;
    std::unordered_map<std::string, std::vector<Pet>> m_classPets;

    // Helper methods
    bool loadLuaFile(const std::string& filePath);
    ClassStats parseClassStats(const std::string& className);
    Spell parseSpell(const std::string& spellName);
    Ability parseAbility(const std::string& abilityName);
    Monster parseMonster(const std::string& monsterName);
    std::vector<Pet> parsePets(const std::string& className);
};

} // namespace CloneMine::RPG
