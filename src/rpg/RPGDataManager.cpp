#include "RPGDataManager.h"
#include <filesystem>
#include <iostream>

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

namespace CloneMine::RPG {

RPGDataManager::RPGDataManager(Plugin::LuaSandbox& sandbox)
    : m_sandbox(sandbox) {}

bool RPGDataManager::loadAllData(const std::string& dataPath) {
    std::cout << "Loading RPG data from: " << dataPath << std::endl;
    
    bool success = true;
    
    // Load in dependency order
    success &= loadSpells(dataPath + "/spells");
    success &= loadAbilities(dataPath + "/abilities");
    success &= loadClasses(dataPath + "/classes");
    success &= loadMonsters(dataPath + "/monsters");
    success &= loadEquipment(dataPath + "/equipment");
    
    if (success) {
        std::cout << "All RPG data loaded successfully!" << std::endl;
        auto stats = getDataStats();
        std::cout << "  Classes: " << stats.classCount << std::endl;
        std::cout << "  Spells: " << stats.spellCount << std::endl;
        std::cout << "  Abilities: " << stats.abilityCount << std::endl;
        std::cout << "  Monsters: " << stats.monsterCount << std::endl;
        std::cout << "  Pets: " << stats.petCount << std::endl;
    } else {
        std::cerr << "Failed to load some RPG data!" << std::endl;
    }
    
    return success;
}

bool RPGDataManager::loadLuaFile(const std::string& filePath) {
    lua_State* L = m_sandbox.getState();
    
    if (luaL_loadfile(L, filePath.c_str()) != LUA_OK) {
        std::cerr << "Failed to load file: " << filePath << std::endl;
        std::cerr << "Error: " << lua_tostring(L, -1) << std::endl;
        lua_pop(L, 1);
        return false;
    }
    
    if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
        std::cerr << "Failed to execute file: " << filePath << std::endl;
        std::cerr << "Error: " << lua_tostring(L, -1) << std::endl;
        lua_pop(L, 1);
        return false;
    }
    
    return true;
}

bool RPGDataManager::loadClasses(const std::string& classPath) {
    std::cout << "Loading classes from: " << classPath << std::endl;
    
    // Class names to load
    std::vector<std::string> classes = {
        "wizard", "sorcerer", "priest", "fighter",
        "ranger", "warlock", "druid", "mastermind", "necromancer"
    };
    
    for (const auto& className : classes) {
        std::string statsFile = classPath + "/" + className + "/" + className + "_stats.lua";
        
        if (!std::filesystem::exists(statsFile)) {
            std::cerr << "Class stats file not found: " << statsFile << std::endl;
            continue;
        }
        
        if (loadLuaFile(statsFile)) {
            ClassStats stats = parseClassStats(className);
            m_classes[className] = stats;
            
            // Load pets if this is a pet class
            std::string petsFile = classPath + "/" + className + "/" + className + "_pets.lua";
            if (std::filesystem::exists(petsFile)) {
                if (loadLuaFile(petsFile)) {
                    m_classPets[className] = parsePets(className);
                }
            }
            
            std::cout << "  Loaded class: " << className << std::endl;
        }
    }
    
    return !m_classes.empty();
}

RPGDataManager::ClassStats RPGDataManager::parseClassStats(const std::string& className) {
    lua_State* L = m_sandbox.getState();
    ClassStats stats;
    stats.name = className;
    
    // Get the global class table
    std::string tableName = className + "Stats";
    lua_getglobal(L, tableName.c_str());
    
    if (lua_istable(L, -1)) {
        // Parse basic stats
        lua_getfield(L, -1, "primaryStat");
        if (lua_isstring(L, -1)) {
            stats.primaryStat = lua_tostring(L, -1);
        }
        lua_pop(L, 1);
        
        lua_getfield(L, -1, "baseHealth");
        if (lua_isnumber(L, -1)) {
            stats.baseHealth = static_cast<int>(lua_tointeger(L, -1));
        }
        lua_pop(L, 1);
        
        lua_getfield(L, -1, "baseResource");
        if (lua_isnumber(L, -1)) {
            stats.baseResource = static_cast<int>(lua_tointeger(L, -1));
        }
        lua_pop(L, 1);
        
        lua_getfield(L, -1, "resourceType");
        if (lua_isstring(L, -1)) {
            stats.resourceType = lua_tostring(L, -1);
        }
        lua_pop(L, 1);
        
        // Parse allowed weapons
        lua_getfield(L, -1, "allowedWeapons");
        if (lua_istable(L, -1)) {
            lua_pushnil(L);
            while (lua_next(L, -2)) {
                if (lua_isstring(L, -1)) {
                    stats.allowedWeapons.push_back(lua_tostring(L, -1));
                }
                lua_pop(L, 1);
            }
        }
        lua_pop(L, 1);
    }
    
    lua_pop(L, 1); // Pop the table
    return stats;
}

bool RPGDataManager::loadSpells(const std::string& spellPath) {
    std::cout << "Loading spells from: " << spellPath << std::endl;
    
    std::string playerSpellsFile = spellPath + "/player_spells.lua";
    std::string monsterSpellsFile = spellPath + "/monster_spells.lua";
    
    bool success = true;
    
    if (std::filesystem::exists(playerSpellsFile)) {
        success &= loadLuaFile(playerSpellsFile);
        std::cout << "  Loaded player spells" << std::endl;
    }
    
    if (std::filesystem::exists(monsterSpellsFile)) {
        success &= loadLuaFile(monsterSpellsFile);
        std::cout << "  Loaded monster spells" << std::endl;
    }
    
    // Parse loaded spells
    lua_State* L = m_sandbox.getState();
    lua_getglobal(L, "playerSpells");
    if (lua_istable(L, -1)) {
        lua_pushnil(L);
        while (lua_next(L, -2)) {
            if (lua_isstring(L, -2)) {
                std::string spellName = lua_tostring(L, -2);
                Spell spell = parseSpell(spellName);
                m_spells[spellName] = spell;
            }
            lua_pop(L, 1);
        }
    }
    lua_pop(L, 1);
    
    return success;
}

RPGDataManager::Spell RPGDataManager::parseSpell(const std::string& spellName) {
    lua_State* L = m_sandbox.getState();
    Spell spell;
    spell.name = spellName;
    
    lua_getglobal(L, "playerSpells");
    if (lua_istable(L, -1)) {
        lua_getfield(L, -1, spellName.c_str());
        if (lua_istable(L, -1)) {
            // Parse spell properties
            lua_getfield(L, -1, "school");
            if (lua_isstring(L, -1)) {
                spell.school = lua_tostring(L, -1);
            }
            lua_pop(L, 1);
            
            lua_getfield(L, -1, "baseDamage");
            if (lua_isnumber(L, -1)) {
                spell.baseDamage = static_cast<int>(lua_tointeger(L, -1));
            }
            lua_pop(L, 1);
            
            lua_getfield(L, -1, "cost");
            if (lua_isnumber(L, -1)) {
                spell.resourceCost = static_cast<int>(lua_tointeger(L, -1));
            }
            lua_pop(L, 1);
            
            lua_getfield(L, -1, "castTime");
            if (lua_isnumber(L, -1)) {
                spell.castTime = lua_tonumber(L, -1);
            }
            lua_pop(L, 1);
            
            lua_getfield(L, -1, "cooldown");
            if (lua_isnumber(L, -1)) {
                spell.cooldown = lua_tonumber(L, -1);
            }
            lua_pop(L, 1);
        }
        lua_pop(L, 1);
    }
    lua_pop(L, 1);
    
    return spell;
}

bool RPGDataManager::loadAbilities(const std::string& abilityPath) {
    std::cout << "Loading abilities from: " << abilityPath << std::endl;
    
    std::string playerAbilitiesFile = abilityPath + "/player_abilities.lua";
    std::string monsterAbilitiesFile = abilityPath + "/monster_abilities.lua";
    
    bool success = true;
    
    if (std::filesystem::exists(playerAbilitiesFile)) {
        success &= loadLuaFile(playerAbilitiesFile);
        std::cout << "  Loaded player abilities" << std::endl;
    }
    
    if (std::filesystem::exists(monsterAbilitiesFile)) {
        success &= loadLuaFile(monsterAbilitiesFile);
        std::cout << "  Loaded monster abilities" << std::endl;
    }
    
    return success;
}

RPGDataManager::Ability RPGDataManager::parseAbility(const std::string& abilityName) {
    Ability ability;
    ability.name = abilityName;
    // TODO: Parse from Lua
    return ability;
}

bool RPGDataManager::loadMonsters(const std::string& monsterPath) {
    std::cout << "Loading monsters from: " << monsterPath << std::endl;
    
    std::string monstersFile = monsterPath + "/monsters_quests.lua";
    
    if (std::filesystem::exists(monstersFile)) {
        bool success = loadLuaFile(monstersFile);
        if (success) {
            std::cout << "  Loaded monsters and quests" << std::endl;
        }
        return success;
    }
    
    return false;
}

RPGDataManager::Monster RPGDataManager::parseMonster(const std::string& monsterName) {
    Monster monster;
    monster.name = monsterName;
    // TODO: Parse from Lua
    return monster;
}

bool RPGDataManager::loadEquipment(const std::string& equipPath) {
    std::cout << "Loading equipment from: " << equipPath << std::endl;
    
    std::string equipmentFile = equipPath + "/equipment_system.lua";
    std::string armorFile = equipPath + "/armor.lua";
    std::string weaponsFile = equipPath + "/weapons.lua";
    
    bool success = true;
    
    if (std::filesystem::exists(equipmentFile)) {
        success &= loadLuaFile(equipmentFile);
        std::cout << "  Loaded equipment system" << std::endl;
    }
    
    if (std::filesystem::exists(armorFile)) {
        success &= loadLuaFile(armorFile);
        std::cout << "  Loaded armor" << std::endl;
    }
    
    if (std::filesystem::exists(weaponsFile)) {
        success &= loadLuaFile(weaponsFile);
        std::cout << "  Loaded weapons" << std::endl;
    }
    
    return success;
}

std::vector<RPGDataManager::Pet> RPGDataManager::parsePets(const std::string& className) {
    std::vector<Pet> pets;
    // TODO: Parse pets from Lua
    return pets;
}

std::optional<RPGDataManager::ClassStats> RPGDataManager::getClass(const std::string& className) const {
    auto it = m_classes.find(className);
    if (it != m_classes.end()) {
        return it->second;
    }
    return std::nullopt;
}

std::optional<RPGDataManager::Spell> RPGDataManager::getSpell(const std::string& spellName) const {
    auto it = m_spells.find(spellName);
    if (it != m_spells.end()) {
        return it->second;
    }
    return std::nullopt;
}

std::optional<RPGDataManager::Ability> RPGDataManager::getAbility(const std::string& abilityName) const {
    auto it = m_abilities.find(abilityName);
    if (it != m_abilities.end()) {
        return it->second;
    }
    return std::nullopt;
}

std::optional<RPGDataManager::Monster> RPGDataManager::getMonster(const std::string& monsterName) const {
    auto it = m_monsters.find(monsterName);
    if (it != m_monsters.end()) {
        return it->second;
    }
    return std::nullopt;
}

std::vector<RPGDataManager::Spell> RPGDataManager::getSpellsForClass(const std::string& className) const {
    std::vector<Spell> result;
    for (const auto& [name, spell] : m_spells) {
        for (const auto& allowedClass : spell.classes) {
            if (allowedClass == className) {
                result.push_back(spell);
                break;
            }
        }
    }
    return result;
}

std::vector<RPGDataManager::Ability> RPGDataManager::getAbilitiesForClass(const std::string& className) const {
    std::vector<Ability> result;
    for (const auto& [name, ability] : m_abilities) {
        for (const auto& allowedClass : ability.classes) {
            if (allowedClass == className) {
                result.push_back(ability);
                break;
            }
        }
    }
    return result;
}

std::vector<RPGDataManager::Pet> RPGDataManager::getPetsForClass(const std::string& className) const {
    auto it = m_classPets.find(className);
    if (it != m_classPets.end()) {
        return it->second;
    }
    return {};
}

std::vector<std::string> RPGDataManager::validate() const {
    std::vector<std::string> errors;
    
    // Validate classes
    for (const auto& [name, classStats] : m_classes) {
        if (classStats.baseHealth <= 0) {
            errors.push_back("Class " + name + " has invalid baseHealth: " + std::to_string(classStats.baseHealth));
        }
        if (classStats.primaryStat.empty()) {
            errors.push_back("Class " + name + " has no primary stat defined");
        }
    }
    
    // Validate spells
    for (const auto& [name, spell] : m_spells) {
        if (spell.baseDamage < 0) {
            errors.push_back("Spell " + name + " has negative baseDamage");
        }
        if (spell.resourceCost < 0) {
            errors.push_back("Spell " + name + " has negative resourceCost");
        }
    }
    
    return errors;
}

RPGDataManager::DataStats RPGDataManager::getDataStats() const {
    DataStats stats;
    stats.classCount = m_classes.size();
    stats.spellCount = m_spells.size();
    stats.abilityCount = m_abilities.size();
    stats.monsterCount = m_monsters.size();
    
    size_t totalPets = 0;
    for (const auto& [className, pets] : m_classPets) {
        totalPets += pets.size();
    }
    stats.petCount = totalPets;
    
    return stats;
}

} // namespace CloneMine::RPG
