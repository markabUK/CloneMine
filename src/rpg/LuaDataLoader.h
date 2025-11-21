#pragma once

#include "CharacterClass.h"
#include "Spell.h"
#include <lua.hpp>
#include <string>
#include <memory>
#include <unordered_map>

namespace RPG {

class LuaDataLoader {
public:
    LuaDataLoader();
    ~LuaDataLoader();
    
    // Prevent copying
    LuaDataLoader(const LuaDataLoader&) = delete;
    LuaDataLoader& operator=(const LuaDataLoader&) = delete;
    
    // Load data from Lua files
    bool loadCharacterClass(const std::string& className);
    bool loadSpell(const std::string& spellName);
    bool loadAllClasses(const std::string& dataDir = "data/classes");
    bool loadAllSpells(const std::string& dataDir = "data/spells");
    
    // Retrieve loaded data
    std::shared_ptr<CharacterClass> getCharacterClass(const std::string& className) const;
    std::shared_ptr<Spell> getSpell(const std::string& spellName) const;
    
    // Utility
    std::vector<std::string> getLoadedClasses() const;
    std::vector<std::string> getLoadedSpells() const;
    bool isClassLoaded(const std::string& className) const;
    bool isSpellLoaded(const std::string& spellName) const;
    
    // Error handling
    const std::string& getLastError() const { return lastError; }
    
private:
    lua_State* L;
    std::unordered_map<std::string, std::shared_ptr<CharacterClass>> classes;
    std::unordered_map<std::string, std::shared_ptr<Spell>> spells;
    std::string lastError;
    
    // Helper methods
    bool executeFile(const std::string& filepath);
    std::shared_ptr<CharacterClass> parseCharacterClass(const std::string& className);
    std::shared_ptr<Spell> parseSpell(const std::string& spellName);
    
    // Lua table parsing helpers
    Stats parseStats(const std::string& tableName);
    ResourcePool parseResourcePool(const std::string& tableName);
    std::vector<DamageScaling> parseDamageScaling(const std::string& tableName);
    DefensiveMechanics parseDefensiveMechanics(const std::string& tableName);
    
    // Lua stack helpers
    std::string getStringField(const char* key, const std::string& defaultValue = "");
    int getIntField(const char* key, int defaultValue = 0);
    float getFloatField(const char* key, float defaultValue = 0.0f);
    bool getBoolField(const char* key, bool defaultValue = false);
    void setError(const std::string& error);
};

} // namespace RPG
