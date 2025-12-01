-- Powerset Loader: Dynamic powerset loading system for CloneMine
-- This module discovers and loads all powerset and class definition files
-- Classes and Powersets are defined in Lua and loaded at runtime

local PowersetLoader = {
    classes = {},
    powersets = {},
    powersetsByCategory = {},
    version = "1.0.0"
}

-- Powerset file paths
local powersetFiles = {
    "data/powersets/attack_powersets.lua",
    "data/powersets/defense_powersets.lua",
    "data/powersets/healing_powersets.lua",
    "data/powersets/buff_powersets.lua",
    "data/powersets/summon_powersets.lua",
    "data/powersets/control_powersets.lua"
}

-- Class definition file
local classFile = "data/powersets/classes.lua"

-- Category constants for validation
local validCategories = {
    "ATTACK", "DEFENSE", "HEALING", "BUFF", "SUMMON", "CONTROL"
}

-- Archetype constants
local validArchetypes = {
    "ATTACKER", "TANK", "HEALER", "MASTERMIND"
}

-- Initialize the powerset loader
function PowersetLoader:init()
    self.classes = {}
    self.powersets = {}
    self.powersetsByCategory = {}
    for _, cat in ipairs(validCategories) do
        self.powersetsByCategory[cat] = {}
    end
end

-- Load a single powerset file
function PowersetLoader:loadPowersetFile(filepath)
    log("Loading powerset file: " .. filepath)
    
    local success, data = pcall(dofile, filepath)
    if not success or not data then
        log("ERROR: Failed to load powerset file: " .. filepath)
        return 0
    end
    
    local count = 0
    for _, powerset in ipairs(data) do
        if self:validatePowerset(powerset) then
            -- Add to main list
            table.insert(self.powersets, powerset)
            
            -- Add to category list
            local cat = powerset.category
            if self.powersetsByCategory[cat] then
                table.insert(self.powersetsByCategory[cat], powerset)
            end
            
            count = count + 1
            log("  Loaded powerset: " .. powerset.name .. " (" .. powerset.category .. ")")
        else
            log("  WARNING: Invalid powerset skipped: " .. (powerset.name or "unknown"))
        end
    end
    
    return count
end

-- Validate a powerset definition
function PowersetLoader:validatePowerset(powerset)
    if not powerset.name or powerset.name == "" then
        return false
    end
    
    if not powerset.category then
        return false
    end
    
    -- Check if category is valid
    local validCat = false
    for _, cat in ipairs(validCategories) do
        if cat == powerset.category then
            validCat = true
            break
        end
    end
    
    if not validCat then
        log("Invalid category: " .. tostring(powerset.category))
        return false
    end
    
    -- Must have abilities
    if not powerset.abilities or #powerset.abilities == 0 then
        log("Powerset has no abilities: " .. powerset.name)
        return false
    end
    
    return true
end

-- Load all powerset files
function PowersetLoader:loadAllPowersets()
    log("=== Loading All Powersets ===")
    
    local totalCount = 0
    for _, filepath in ipairs(powersetFiles) do
        local count = self:loadPowersetFile(filepath)
        totalCount = totalCount + count
    end
    
    log("Loaded " .. totalCount .. " powersets total")
    return totalCount
end

-- Load class definitions
function PowersetLoader:loadClasses()
    log("=== Loading Class Definitions ===")
    
    local success, data = pcall(dofile, classFile)
    if not success or not data then
        log("ERROR: Failed to load class file: " .. classFile)
        return 0
    end
    
    local count = 0
    for _, class in ipairs(data) do
        if self:validateClass(class) then
            self.classes[class.name] = class
            count = count + 1
            log("  Loaded class: " .. class.name .. " (" .. class.archetype .. ")")
        else
            log("  WARNING: Invalid class skipped: " .. (class.name or "unknown"))
        end
    end
    
    log("Loaded " .. count .. " classes total")
    return count
end

-- Validate a class definition
function PowersetLoader:validateClass(class)
    if not class.name or class.name == "" then
        return false
    end
    
    if not class.archetype then
        return false
    end
    
    -- Check if archetype is valid
    local validArch = false
    for _, arch in ipairs(validArchetypes) do
        if arch == class.archetype then
            validArch = true
            break
        end
    end
    
    if not validArch then
        log("Invalid archetype: " .. tostring(class.archetype))
        return false
    end
    
    return true
end

-- Load everything
function PowersetLoader:loadAll()
    self:init()
    local powersetCount = self:loadAllPowersets()
    local classCount = self:loadClasses()
    return powersetCount, classCount
end

-- Get a powerset by name
function PowersetLoader:getPowerset(name)
    for _, powerset in ipairs(self.powersets) do
        if powerset.name == name then
            return powerset
        end
    end
    return nil
end

-- Get all powersets
function PowersetLoader:getAllPowersets()
    return self.powersets
end

-- Get powersets by category
function PowersetLoader:getPowersetsByCategory(category)
    return self.powersetsByCategory[category] or {}
end

-- Get a class by name
function PowersetLoader:getClass(name)
    return self.classes[name]
end

-- Get all classes
function PowersetLoader:getAllClasses()
    return self.classes
end

-- Get classes by archetype
function PowersetLoader:getClassesByArchetype(archetype)
    local result = {}
    for name, class in pairs(self.classes) do
        if class.archetype == archetype then
            table.insert(result, class)
        end
    end
    return result
end

-- Get class names for UI
function PowersetLoader:getClassNames()
    local names = {}
    for name, _ in pairs(self.classes) do
        table.insert(names, name)
    end
    table.sort(names)
    return names
end

-- Get available primary powersets for a class
function PowersetLoader:getAvailablePrimaryPowersets(className)
    local class = self.classes[className]
    if not class then
        return {}
    end
    
    local result = {}
    for _, powersetName in ipairs(class.allowedPrimaryPowersets or {}) do
        local powerset = self:getPowerset(powersetName)
        if powerset then
            table.insert(result, powerset)
        end
    end
    return result
end

-- Get available secondary powersets for a class
function PowersetLoader:getAvailableSecondaryPowersets(className)
    local class = self.classes[className]
    if not class then
        return {}
    end
    
    local result = {}
    for _, powersetName in ipairs(class.allowedSecondaryPowersets or {}) do
        local powerset = self:getPowerset(powersetName)
        if powerset then
            table.insert(result, powerset)
        end
    end
    return result
end

-- Check if a powerset selection is valid for a class
function PowersetLoader:isValidSelection(className, primaryPowerset, secondaryPowerset)
    local class = self.classes[className]
    if not class then
        return false, "Class not found: " .. className
    end
    
    -- Check primary
    local validPrimary = false
    for _, name in ipairs(class.allowedPrimaryPowersets or {}) do
        if name == primaryPowerset then
            validPrimary = true
            break
        end
    end
    if not validPrimary then
        return false, "Invalid primary powerset for class"
    end
    
    -- Check secondary
    local validSecondary = false
    for _, name in ipairs(class.allowedSecondaryPowersets or {}) do
        if name == secondaryPowerset then
            validSecondary = true
            break
        end
    end
    if not validSecondary then
        return false, "Invalid secondary powerset for class"
    end
    
    return true, "Valid selection"
end

-- Get secondary effectiveness for a class
function PowersetLoader:getSecondaryEffectiveness(className)
    local class = self.classes[className]
    if class then
        return class.secondaryEffectiveness or 0.75
    end
    return 0.75
end

-- Get powerset summary for UI display
function PowersetLoader:getPowersetSummary(powersetName)
    local powerset = self:getPowerset(powersetName)
    if not powerset then
        return "Unknown powerset"
    end
    
    return string.format("%s (%s): %s - %d abilities", 
        powerset.name, 
        powerset.category,
        powerset.description or "No description",
        #powerset.abilities)
end

-- Get class summary for UI display
function PowersetLoader:getClassSummary(className)
    local class = self.classes[className]
    if not class then
        return "Unknown class"
    end
    
    return string.format("%s (%s): %s", 
        class.name, 
        class.archetype,
        class.description or "No description")
end

-- Export powerset data as table (for C++ interop)
function PowersetLoader:exportPowersetsToTable()
    local result = {}
    for _, powerset in ipairs(self.powersets) do
        table.insert(result, {
            name = powerset.name,
            description = powerset.description or "",
            category = powerset.category,
            damageType = powerset.damageType or "NONE",
            abilityCount = #powerset.abilities,
            abilities = powerset.abilities
        })
    end
    return result
end

-- Export class data as table (for C++ interop)
function PowersetLoader:exportClassesToTable()
    local result = {}
    for name, class in pairs(self.classes) do
        table.insert(result, {
            name = class.name,
            description = class.description or "",
            archetype = class.archetype,
            primaryStat = class.primaryStat,
            resourceType = class.resourceType,
            baseHealth = class.baseHealth,
            healthPerLevel = class.healthPerLevel,
            baseResource = class.baseResource,
            resourcePerLevel = class.resourcePerLevel,
            secondaryEffectiveness = class.secondaryEffectiveness,
            allowedPrimaryPowersets = class.allowedPrimaryPowersets or {},
            allowedSecondaryPowersets = class.allowedSecondaryPowersets or {}
        })
    end
    return result
end

return PowersetLoader
