-- Class Loader: Dynamic class loading system for CloneMine
-- This module discovers and loads all character class files automatically

local ClassLoader = {
    classes = {},
    classDirectories = {},
    version = "1.0.0"
}

-- Initialize the class loader
function ClassLoader:init()
    self.classes = {}
    self.classDirectories = {
        "data/classes/wizard",
        "data/classes/sorcerer",
        "data/classes/priest",
        "data/classes/fighter",
        "data/classes/ranger",
        "data/classes/warlock",
        "data/classes/druid",
        "data/classes/mastermind",
        "data/classes/necromancer"
    }
end

-- Load a single class from its directory
function ClassLoader:loadClass(classDir)
    local className = classDir:match("([^/]+)$"):upper()
    
    log("Loading class: " .. className)
    
    local classData = {
        name = className,
        stats = nil,
        spells = {},
        abilities = {},
        feats = {},
        summoning = nil
    }
    
    -- Load stats file (required)
    local statsPath = classDir .. "/" .. className:lower() .. "_stats.lua"
    local success, stats = pcall(dofile, statsPath)
    if success and stats then
        classData.stats = stats
    else
        log("ERROR: Failed to load stats for " .. className)
        return false
    end
    
    -- Load spells file (required)
    local spellsPath = classDir .. "/" .. className:lower() .. "_spells.lua"
    success, spells = pcall(dofile, spellsPath)
    if success and spells then
        classData.spells = spells
    end
    
    -- Load abilities file (required)
    local abilitiesPath = classDir .. "/" .. className:lower() .. "_abilities.lua"
    success, abilities = pcall(dofile, abilitiesPath)
    if success and abilities then
        classData.abilities = abilities
    end
    
    -- Load feats file (optional)
    local featsPath = classDir .. "/" .. className:lower() .. "_feats.lua"
    success, feats = pcall(dofile, featsPath)
    if success and feats then
        classData.feats = feats
    end
    
    -- Load summoning file (optional, only for pet classes)
    local summoningPath = classDir .. "/" .. className:lower() .. "_summoning.lua"
    success, summoning = pcall(dofile, summoningPath)
    if success and summoning then
        classData.summoning = summoning
    end
    
    -- Register the class
    self.classes[className] = classData
    log("Successfully loaded class: " .. className)
    return true
end

-- Load all classes from their directories
function ClassLoader:loadAllClasses()
    log("=== Loading All Character Classes ===")
    
    local loadCount = 0
    for _, classDir in ipairs(self.classDirectories) do
        if self:loadClass(classDir) then
            loadCount = loadCount + 1
        end
    end
    
    log("Loaded " .. loadCount .. " character classes")
    return loadCount
end

-- Get a loaded class by name
function ClassLoader:getClass(className)
    return self.classes[className:upper()]
end

-- Get all loaded classes
function ClassLoader:getAllClasses()
    return self.classes
end

-- Check if a class exists
function ClassLoader:classExists(className)
    return self.classes[className:upper()] ~= nil
end

-- Get class stats
function ClassLoader:getClassStats(className)
    local class = self:getClass(className)
    return class and class.stats or nil
end

-- Get class spells
function ClassLoader:getClassSpells(className)
    local class = self:getClass(className)
    return class and class.spells or {}
end

-- Get class abilities
function ClassLoader:getClassAbilities(className)
    local class = self:getClass(className)
    return class and class.abilities or {}
end

-- Get class feats
function ClassLoader:getClassFeats(className)
    local class = self:getClass(className)
    return class and class.feats or {}
end

-- Get class summoning data
function ClassLoader:getClassSummoning(className)
    local class = self:getClass(className)
    return class and class.summoning or nil
end

-- Check if a class can summon pets
function ClassLoader:canClassSummon(className)
    local class = self:getClass(className)
    return class and class.summoning ~= nil
end

-- Get max summons for a class
function ClassLoader:getMaxSummons(className)
    local summoning = self:getClassSummoning(className)
    return summoning and summoning.maxSummons or 0
end

-- Add a custom spell to a class (for plugins)
function ClassLoader:addSpellToClass(className, spell)
    local class = self:getClass(className)
    if class then
        table.insert(class.spells, spell)
        log("Added custom spell to " .. className .. ": " .. spell.name)
        return true
    end
    return false
end

-- Add a custom ability to a class (for plugins)
function ClassLoader:addAbilityToClass(className, ability)
    local class = self:getClass(className)
    if class then
        table.insert(class.abilities, ability)
        log("Added custom ability to " .. className .. ": " .. ability.name)
        return true
    end
    return false
end

-- Register a completely new class (for plugins)
function ClassLoader:registerClass(className, classDir)
    table.insert(self.classDirectories, classDir)
    return self:loadClass(classDir)
end

-- Get class list for UI
function ClassLoader:getClassNames()
    local names = {}
    for name, _ in pairs(self.classes) do
        table.insert(names, name)
    end
    table.sort(names)
    return names
end

-- Get class description
function ClassLoader:getClassDescription(className)
    local stats = self:getClassStats(className)
    if stats then
        return string.format("%s (%s): %s", 
            stats.name, 
            stats.primaryStat, 
            stats.description or "No description available")
    end
    return "Unknown class"
end

return ClassLoader
