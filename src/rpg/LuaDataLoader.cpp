#include "LuaDataLoader.h"
#include <filesystem>
#include <iostream>

namespace RPG {

LuaDataLoader::LuaDataLoader() {
    L = luaL_newstate();
    luaL_openlibs(L);
}

LuaDataLoader::~LuaDataLoader() {
    if (L) {
        lua_close(L);
    }
}

bool LuaDataLoader::executeFile(const std::string& filepath) {
    if (luaL_dofile(L, filepath.c_str()) != LUA_OK) {
        setError(std::string("Failed to load file: ") + lua_tostring(L, -1));
        lua_pop(L, 1);
        return false;
    }
    return true;
}

void LuaDataLoader::setError(const std::string& error) {
    lastError = error;
    std::cerr << "LuaDataLoader Error: " << error << std::endl;
}

std::string LuaDataLoader::getStringField(const char* key, const std::string& defaultValue) {
    lua_getfield(L, -1, key);
    std::string result = defaultValue;
    if (lua_isstring(L, -1)) {
        result = lua_tostring(L, -1);
    }
    lua_pop(L, 1);
    return result;
}

int LuaDataLoader::getIntField(const char* key, int defaultValue) {
    lua_getfield(L, -1, key);
    int result = defaultValue;
    if (lua_isnumber(L, -1)) {
        result = static_cast<int>(lua_tointeger(L, -1));
    }
    lua_pop(L, 1);
    return result;
}

float LuaDataLoader::getFloatField(const char* key, float defaultValue) {
    lua_getfield(L, -1, key);
    float result = defaultValue;
    if (lua_isnumber(L, -1)) {
        result = static_cast<float>(lua_tonumber(L, -1));
    }
    lua_pop(L, 1);
    return result;
}

bool LuaDataLoader::getBoolField(const char* key, bool defaultValue) {
    lua_getfield(L, -1, key);
    bool result = defaultValue;
    if (lua_isboolean(L, -1)) {
        result = lua_toboolean(L, -1);
    }
    lua_pop(L, 1);
    return result;
}

Stats LuaDataLoader::parseStats(const std::string& tableName) {
    Stats stats;
    lua_getglobal(L, tableName.c_str());
    if (lua_istable(L, -1)) {
        stats.strength = getIntField("STR", 10);
        stats.dexterity = getIntField("DEX", 10);
        stats.constitution = getIntField("CON", 10);
        stats.intelligence = getIntField("INT", 10);
        stats.wisdom = getIntField("WIS", 10);
        stats.charisma = getIntField("CHA", 10);
    }
    lua_pop(L, 1);
    return stats;
}

ResourcePool LuaDataLoader::parseResourcePool(const std::string& tableName) {
    ResourcePool pool;
    lua_getglobal(L, tableName.c_str());
    if (lua_istable(L, -1)) {
        pool.name = getStringField("name", "Mana");
        pool.maxValue = getIntField("maxValue", 100);
        pool.currentValue = getIntField("currentValue", 100);
        pool.combatRegen = getFloatField("combatRegen", 2.0f);
        pool.outOfCombatRegen = getFloatField("outOfCombatRegen", 10.0f);
    }
    lua_pop(L, 1);
    return pool;
}

DefensiveMechanics LuaDataLoader::parseDefensiveMechanics(const std::string& tableName) {
    DefensiveMechanics mechanics;
    lua_getglobal(L, tableName.c_str());
    if (lua_istable(L, -1)) {
        mechanics.canDodge = getBoolField("canDodge", false);
        mechanics.canParry = getBoolField("canParry", false);
        mechanics.canBlock = getBoolField("canBlock", false);
    }
    lua_pop(L, 1);
    return mechanics;
}

std::vector<DamageScaling> LuaDataLoader::parseDamageScaling(const std::string& tableName) {
    std::vector<DamageScaling> scalingList;
    
    lua_getglobal(L, tableName.c_str());
    if (lua_istable(L, -1)) {
        lua_pushnil(L);
        while (lua_next(L, -2) != 0) {
            if (lua_istable(L, -1)) {
                DamageScaling scaling;
                scaling.stat = getStringField("stat", "");
                scaling.damageType = getStringField("damageType", "");
                scaling.scalingFactor = getFloatField("scalingFactor", 0.0f);
                scalingList.push_back(scaling);
            }
            lua_pop(L, 1);
        }
    }
    lua_pop(L, 1);
    
    return scalingList;
}

bool LuaDataLoader::loadCharacterClass(const std::string& className) {
    std::string filepath = "data/classes/" + className + "/" + className + "_stats.lua";
    
    if (!executeFile(filepath)) {
        return false;
    }
    
    auto charClass = parseCharacterClass(className);
    if (charClass) {
        classes[className] = charClass;
        return true;
    }
    
    return false;
}

std::shared_ptr<CharacterClass> LuaDataLoader::parseCharacterClass(const std::string& className) {
    auto charClass = std::make_shared<CharacterClass>();
    charClass->setName(className);
    
    // Parse from the Lua global table (class data)
    lua_getglobal(L, className.c_str());
    if (!lua_istable(L, -1)) {
        setError("Class table not found: " + className);
        lua_pop(L, 1);
        return nullptr;
    }
    
    charClass->setPrimaryStat(getStringField("primaryStat", "STR"));
    charClass->setBaseStats(parseStats("baseStats"));
    charClass->setResourcePool(parseResourcePool("resourcePool"));
    charClass->setDefensiveMechanics(parseDefensiveMechanics("defensiveMechanics"));
    
    // Parse damage scaling
    auto scaling = parseDamageScaling("damageScaling");
    for (const auto& s : scaling) {
        charClass->addDamageScaling(s);
    }
    
    lua_pop(L, 1);
    return charClass;
}

bool LuaDataLoader::loadAllClasses(const std::string& dataDir) {
    namespace fs = std::filesystem;
    
    if (!fs::exists(dataDir)) {
        setError("Classes directory not found: " + dataDir);
        return false;
    }
    
    int loadedCount = 0;
    for (const auto& entry : fs::directory_iterator(dataDir)) {
        if (entry.is_directory()) {
            std::string className = entry.path().filename().string();
            if (loadCharacterClass(className)) {
                loadedCount++;
            }
        }
    }
    
    return loadedCount > 0;
}

std::shared_ptr<CharacterClass> LuaDataLoader::getCharacterClass(const std::string& className) const {
    auto it = classes.find(className);
    if (it != classes.end()) {
        return it->second;
    }
    return nullptr;
}

bool LuaDataLoader::isClassLoaded(const std::string& className) const {
    return classes.find(className) != classes.end();
}

std::vector<std::string> LuaDataLoader::getLoadedClasses() const {
    std::vector<std::string> result;
    for (const auto& pair : classes) {
        result.push_back(pair.first);
    }
    return result;
}

// Placeholder implementations for spell loading
bool LuaDataLoader::loadSpell(const std::string& spellName) {
    // TODO: Implement spell loading
    return false;
}

bool LuaDataLoader::loadAllSpells(const std::string& dataDir) {
    // TODO: Implement bulk spell loading
    return false;
}

std::shared_ptr<Spell> LuaDataLoader::getSpell(const std::string& spellName) const {
    auto it = spells.find(spellName);
    if (it != spells.end()) {
        return it->second;
    }
    return nullptr;
}

bool LuaDataLoader::isSpellLoaded(const std::string& spellName) const {
    return spells.find(spellName) != spells.end();
}

std::vector<std::string> LuaDataLoader::getLoadedSpells() const {
    std::vector<std::string> result;
    for (const auto& pair : spells) {
        result.push_back(pair.first);
    }
    return result;
}

} // namespace RPG
