-- Content Loader: Master loader for all game content
-- Initializes and loads all core game data at startup

local ContentLoader = {
    version = "1.0.0",
    loaded = false
}

-- Initialize the content loader
function ContentLoader:init()
    log("=== Initializing Content Loader v" .. self.version .. " ===")
    
    self.classLoader = nil
    self.spells = {}
    self.abilities = {}
    self.monsters = {}
    self.equipment = {}
    self.encounters = nil
    
    self.loaded = false
end

-- Load all content
function ContentLoader:loadAll()
    log("Loading all game content...")
    
    local success = true
    
    -- Load class system
    success = self:loadClasses() and success
    
    -- Load spells (player and monster)
    success = self:loadSpells() and success
    
    -- Load abilities (player and monster)
    success = self:loadAbilities() and success
    
    -- Load monsters
    success = self:loadMonsters() and success
    
    -- Load equipment
    success = self:loadEquipment() and success
    
    -- Load encounters
    success = self:loadEncounters() and success
    
    if success then
        self.loaded = true
        log("=== All content loaded successfully ===")
    else
        log("ERROR: Some content failed to load")
    end
    
    return success
end

-- Load character classes
function ContentLoader:loadClasses()
    log("Loading character classes...")
    
    local success, ClassLoader = pcall(require, "data/class_loader")
    if not success then
        log("ERROR: Failed to load class loader")
        return false
    end
    
    ClassLoader:init()
    local count = ClassLoader:loadAllClasses()
    
    if count > 0 then
        self.classLoader = ClassLoader
        log("Loaded " .. count .. " character classes")
        return true
    end
    
    return false
end

-- Load spells
function ContentLoader:loadSpells()
    log("Loading spells...")
    
    -- Load player spells
    local success, playerSpells = pcall(dofile, "data/spells/player_spells.lua")
    if success and playerSpells then
        self.spells.player = playerSpells
        log("Loaded " .. #playerSpells .. " player spells")
    else
        log("WARNING: Failed to load player spells")
    end
    
    -- Load monster spells
    success, monsterSpells = pcall(dofile, "data/spells/monster_spells.lua")
    if success and monsterSpells then
        self.spells.monster = monsterSpells
        log("Loaded " .. #monsterSpells .. " monster spells")
    else
        log("WARNING: Failed to load monster spells")
    end
    
    return self.spells.player ~= nil or self.spells.monster ~= nil
end

-- Load abilities
function ContentLoader:loadAbilities()
    log("Loading abilities...")
    
    -- Load player abilities
    local success, playerAbilities = pcall(dofile, "data/abilities/player_abilities.lua")
    if success and playerAbilities then
        self.abilities.player = playerAbilities
        log("Loaded " .. #playerAbilities .. " player abilities")
    else
        log("WARNING: Failed to load player abilities")
    end
    
    -- Load monster abilities
    success, monsterAbilities = pcall(dofile, "data/abilities/monster_abilities.lua")
    if success and monsterAbilities then
        self.abilities.monster = monsterAbilities
        log("Loaded " .. #monsterAbilities .. " monster abilities")
    else
        log("WARNING: Failed to load monster abilities")
    end
    
    return self.abilities.player ~= nil or self.abilities.monster ~= nil
end

-- Load monsters
function ContentLoader:loadMonsters()
    log("Loading monsters...")
    
    -- This would load from data/monsters/monster_registry.lua
    -- For now, we'll use the existing monsters_quests.lua from plugins
    local success, monsters = pcall(dofile, "plugins/monsters_quests.lua")
    if success and monsters then
        self.monsters = monsters
        log("Loaded monster data")
        return true
    end
    
    log("WARNING: Failed to load monsters")
    return false
end

-- Load equipment
function ContentLoader:loadEquipment()
    log("Loading equipment...")
    
    -- This would load from data/equipment/*.lua
    -- For now, we'll use the existing equipment_system.lua from plugins
    local success, equipment = pcall(dofile, "plugins/equipment_system.lua")
    if success and equipment then
        self.equipment = equipment
        log("Loaded equipment data")
        return true
    end
    
    log("WARNING: Failed to load equipment")
    return false
end

-- Load encounter system
function ContentLoader:loadEncounters()
    log("Loading encounter system...")
    
    local success, encounters = pcall(require, "data/encounters/encounter_tables")
    if success and encounters then
        self.encounters = encounters
        log("Loaded encounter tables")
        return true
    end
    
    log("WARNING: Failed to load encounters")
    return false
end

-- Getters for loaded content

function ContentLoader:getClassLoader()
    return self.classLoader
end

function ContentLoader:getPlayerSpells()
    return self.spells.player or {}
end

function ContentLoader:getMonsterSpells()
    return self.spells.monster or {}
end

function ContentLoader:getPlayerAbilities()
    return self.abilities.player or {}
end

function ContentLoader:getMonsterAbilities()
    return self.abilities.monster or {}
end

function ContentLoader:getMonsters()
    return self.monsters
end

function ContentLoader:getEquipment()
    return self.equipment
end

function ContentLoader:getEncounters()
    return self.encounters
end

-- Check if content is loaded
function ContentLoader:isLoaded()
    return self.loaded
end

-- Get loaded content summary
function ContentLoader:getSummary()
    if not self.loaded then
        return "Content not loaded"
    end
    
    local summary = "=== Content Summary ===\n"
    
    if self.classLoader then
        local classes = self.classLoader:getClassNames()
        summary = summary .. "Classes: " .. #classes .. " loaded\n"
    end
    
    if self.spells.player then
        summary = summary .. "Player Spells: " .. #self.spells.player .. "\n"
    end
    
    if self.spells.monster then
        summary = summary .. "Monster Spells: " .. #self.spells.monster .. "\n"
    end
    
    if self.abilities.player then
        summary = summary .. "Player Abilities: " .. #self.abilities.player .. "\n"
    end
    
    if self.abilities.monster then
        summary = summary .. "Monster Abilities: " .. #self.abilities.monster .. "\n"
    end
    
    summary = summary .. "Monsters: Loaded\n"
    summary = summary .. "Equipment: Loaded\n"
    summary = summary .. "Encounters: Loaded\n"
    
    return summary
end

-- Reload all content (for development)
function ContentLoader:reload()
    log("Reloading all content...")
    self:init()
    return self:loadAll()
end

return ContentLoader
