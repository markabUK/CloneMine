-- D&D-Style Character Classes Plugin
-- Implements class system with specializations
-- Abilities have been moved to abilities.lua for better organization
-- Level cap: 20 (expandable to 120 in future)

-- Constants
local MAX_LEVEL = 20  -- Current level cap (will increase to 120 later)
local STAT_POINTS_PER_LEVEL = 3

-- D&D Stats
local Stats = {
    STRENGTH = "STR",      -- Physical power, melee damage
    DEXTERITY = "DEX",     -- Agility, ranged attacks, AC
    CONSTITUTION = "CON",  -- Health, stamina
    INTELLIGENCE = "INT",  -- Wizard spells, knowledge
    WISDOM = "WIS",        -- Priest spells, perception
    CHARISMA = "CHA"       -- Sorcerer spells, persuasion
}

-- Character Classes
local CharacterClass = {
    WIZARD = {
        name = "Wizard",
        description = "Masters of arcane magic through study",
        primaryStat = Stats.INTELLIGENCE,
        resourceType = "Mana",
        baseResource = 100,
        resourcePerLevel = 15,
        hitDieSize = 6,
        baseHealth = 40,
        healthPerLevel = 6,
        
        allowedWeapons = {"staff", "wand", "dagger"},
        allowedArmor = {"cloth"},
        canDualWield = false,
        canUseShield = false,
        
        startingStats = {
            STR = 8, DEX = 10, CON = 10,
            INT = 16, WIS = 12, CHA = 10
        }
    },
    
    SORCERER = {
        name = "Sorcerer",
        description = "Innate spellcasters powered by raw charisma",
        primaryStat = Stats.CHARISMA,
        resourceType = "Mana",
        baseResource = 120,
        resourcePerLevel = 18,
        hitDieSize = 6,
        baseHealth = 45,
        healthPerLevel = 6,
        
        allowedWeapons = {"staff", "wand", "dagger"},
        allowedArmor = {"cloth"},
        canDualWield = false,
        canUseShield = false,
        
        startingStats = {
            STR = 8, DEX = 10, CON = 11,
            INT = 10, WIS = 10, CHA = 16
        }
    },
    
    PRIEST = {
        name = "Priest",
        description = "Divine healers and supporters",
        primaryStat = Stats.WISDOM,
        resourceType = "Mana",
        baseResource = 90,
        resourcePerLevel = 12,
        hitDieSize = 8,
        baseHealth = 50,
        healthPerLevel = 8,
        
        allowedWeapons = {"mace", "staff", "wand"},
        allowedArmor = {"cloth", "leather", "mail"},
        canDualWield = false,
        canUseShield = true,
        
        startingStats = {
            STR = 10, DEX = 8, CON = 12,
            INT = 10, WIS = 16, CHA = 12
        }
    },
    
    FIGHTER = {
        name = "Fighter",
        description = "Master of weapons and combat",
        primaryStat = Stats.STRENGTH,
        resourceType = "Energy",
        baseResource = 100,
        resourcePerLevel = 10,
        hitDieSize = 10,
        baseHealth = 70,
        healthPerLevel = 10,
        
        allowedWeapons = {"sword", "axe", "mace", "spear", "bow", "crossbow"},
        allowedArmor = {"cloth", "leather", "mail", "plate"},
        canDualWield = true,
        canUseShield = true,
        
        startingStats = {
            STR = 16, DEX = 12, CON = 14,
            INT = 8, WIS = 10, CHA = 10
        }
    },
    
    RANGER = {
        name = "Ranger",
        description = "Nature warrior with animal companion",
        primaryStat = Stats.DEXTERITY,
        resourceType = "Focus",
        baseResource = 100,
        resourcePerLevel = 10,
        hitDieSize = 10,
        baseHealth = 60,
        healthPerLevel = 10,
        
        allowedWeapons = {"bow", "crossbow", "sword", "dagger", "spear"},
        allowedArmor = {"cloth", "leather", "mail"},
        canDualWield = true,
        canUseShield = false,
        hasPet = true,
        
        startingStats = {
            STR = 12, DEX = 16, CON = 12,
            INT = 10, WIS = 14, CHA = 8
        }
    },
    
    WARLOCK = {
        name = "Warlock",
        description = "Dark magic wielder with demonic pets",
        primaryStat = Stats.CHARISMA,
        resourceType = "Soul Energy",
        baseResource = 100,
        resourcePerLevel = 12,
        hitDieSize = 8,
        baseHealth = 50,
        healthPerLevel = 8,
        
        allowedWeapons = {"staff", "wand", "dagger"},
        allowedArmor = {"cloth", "leather"},
        canDualWield = false,
        canUseShield = false,
        hasPet = true,
        
        startingStats = {
            STR = 8, DEX = 10, CON = 12,
            INT = 12, WIS = 10, CHA = 16
        }
    },
    
    DRUID = {
        name = "Druid",
        description = "Shape-shifting nature guardian",
        primaryStat = Stats.WISDOM,
        resourceType = "Nature Power",
        baseResource = 100,
        resourcePerLevel = 12,
        hitDieSize = 8,
        baseHealth = 55,
        healthPerLevel = 8,
        
        allowedWeapons = {"staff", "dagger", "mace"},
        allowedArmor = {"cloth", "leather"},
        canDualWield = false,
        canUseShield = false,
        canShapeshift = true,
        
        startingStats = {
            STR = 10, DEX = 12, CON = 12,
            INT = 10, WIS = 16, CHA = 10
        }
    },
    
    MASTERMIND = {
        name = "Mastermind",
        description = "Pet-focused controller and support, inspired by City of Villains",
        primaryStat = Stats.CHARISMA,
        resourceType = "Control",
        baseResource = 100,
        resourcePerLevel = 12,
        hitDieSize = 6,
        baseHealth = 45,
        healthPerLevel = 6,
        
        allowedWeapons = {"wand", "staff", "dagger"},
        allowedArmor = {"cloth", "leather"},
        canDualWield = false,
        canUseShield = false,
        hasPets = true,
        maxPets = 3,  -- Can control multiple pets!
        
        startingStats = {
            STR = 8, DEX = 10, CON = 10,
            INT = 12, WIS = 10, CHA = 16
        },
        
        petTypes = {
            minion = {health = 50, damage = 10, ability = "Scratch"},
            lieutenant = {health = 100, damage = 20, ability = "Power Strike"},
            boss = {health = 200, damage = 35, ability = "Devastate"}
        }
    }
}

-- Player Character with Class
local PlayerCharacter = {
    class = nil,
    level = 1,
    experience = 0,
    experienceToNextLevel = 1000,
    
    -- Stats
    stats = {
        STR = 10, DEX = 10, CON = 10,
        INT = 10, WIS = 10, CHA = 10
    },
    
    -- Resources
    currentResource = 100,
    maxResource = 100,
    resourceRegenInCombat = 2,    -- per second
    resourceRegenOutCombat = 10,  -- per second
    inCombat = false,
    
    -- Health
    currentHealth = 50,
    maxHealth = 50,
    isDead = false,
    isGhost = false,  -- Ghost form when dead
    
    -- Shapeshift state (Druid only)
    currentShapeshift = nil,  -- Current form, nil = normal form
    shapeshiftedForm = nil,   -- Details of current form
    
    -- Buffs (cleared on death unless specified otherwise)
    activeBuffs = {},
    
    -- Available stat points
    availableStatPoints = 0,
    
    -- Unlocked abilities (reference abilities.lua)
    unlockedAbilities = {}
}

function onLoad()
    log("Character Classes Plugin loaded!")
    log("Level cap: " .. MAX_LEVEL)
    log("Available classes:")
    for className, class in pairs(CharacterClass) do
        log("  - " .. class.name .. " (" .. class.description .. ")")
        log("    Primary: " .. class.primaryStat .. " | Resource: " .. class.resourceType)
    end
    log("Note: Abilities are defined in abilities.lua")
end

function onUpdate(deltaTime)
    -- Skip updates if in ghost form
    if PlayerCharacter.isGhost then
        return
    end
    
    -- Regenerate resource
    regenerateResource(deltaTime)
    
    -- Check for combat timeout
    updateCombatStatus(deltaTime)
end

function setPlayerClass(className)
    local class = CharacterClass[className]
    if not class then
        log("Invalid class: " .. className)
        return false
    end
    
    PlayerCharacter.class = class
    PlayerCharacter.stats = {}
    for stat, value in pairs(class.startingStats) do
        PlayerCharacter.stats[stat] = value
    end
    
    -- Calculate initial resources
    PlayerCharacter.maxHealth = class.baseHealth
    PlayerCharacter.currentHealth = PlayerCharacter.maxHealth
    PlayerCharacter.maxResource = class.baseResource
    PlayerCharacter.currentResource = PlayerCharacter.maxResource
    
    -- Unlock level 1 abilities (from abilities.lua)
    unlockAbilitiesForLevel(1)
    
    log("Class set to: " .. class.name)
    log("Starting stats: STR=" .. PlayerCharacter.stats.STR .. 
        " DEX=" .. PlayerCharacter.stats.DEX .. 
        " CON=" .. PlayerCharacter.stats.CON ..
        " INT=" .. PlayerCharacter.stats.INT ..
        " WIS=" .. PlayerCharacter.stats.WIS ..
        " CHA=" .. PlayerCharacter.stats.CHA)
    
    return true
end

function regenerateResource(deltaTime)
    if not PlayerCharacter.class then return end
    
    local regenRate = PlayerCharacter.inCombat and 
                     PlayerCharacter.resourceRegenInCombat or 
                     PlayerCharacter.resourceRegenOutCombat
    
    PlayerCharacter.currentResource = math.min(
        PlayerCharacter.maxResource,
        PlayerCharacter.currentResource + (regenRate * deltaTime)
    )
end

function updateCombatStatus(deltaTime)
    -- Combat timeout logic would go here
    -- For now, manually managed
end

function enterCombat()
    PlayerCharacter.inCombat = true
    log("Entered combat - resource regen reduced")
end

function leaveCombat()
    PlayerCharacter.inCombat = false
    log("Left combat - resource regen increased")
end

function castAbility(abilityName)
    if not PlayerCharacter.class then
        log("No class selected!")
        return false
    end
    
    if PlayerCharacter.isGhost or PlayerCharacter.isDead then
        log("Cannot cast abilities while dead!")
        return false
    end
    
    -- Find ability (would reference abilities.lua in practice)
    -- This is a simplified version for the plugin system
    local ability = findAbilityByName(abilityName)
    
    if not ability then
        log("Ability not found: " .. abilityName)
        return false
    end
    
    -- Check if unlocked
    if not PlayerCharacter.unlockedAbilities[abilityName] then
        log("Ability not unlocked: " .. abilityName)
        return false
    end
    
    -- Check resource cost
    if PlayerCharacter.currentResource < ability.cost then
        log("Not enough " .. PlayerCharacter.class.resourceType .. 
            " (need " .. ability.cost .. ", have " .. 
            math.floor(PlayerCharacter.currentResource) .. ")")
        return false
    end
    
    -- Cast ability
    PlayerCharacter.currentResource = PlayerCharacter.currentResource - ability.cost
    
    log("Cast " .. abilityName .. " (-" .. ability.cost .. " " .. 
        PlayerCharacter.class.resourceType .. ")")
    
    if ability.damage then
        log("  Dealt " .. ability.damage .. " damage")
    end
    if ability.healing then
        log("  Healed " .. ability.healing .. " health")
    end
    
    enterCombat()
    return true
end

function findAbilityByName(name)
    -- This would query abilities.lua in practice
    -- Simplified for plugin demonstration
    return {name = name, cost = 10, damage = 20}
end

function addExperience(amount)
    PlayerCharacter.experience = PlayerCharacter.experience + amount
    log("Gained " .. amount .. " experience")
    
    while PlayerCharacter.experience >= PlayerCharacter.experienceToNextLevel do
        levelUp()
    end
end

function levelUp()
    if not PlayerCharacter.class then return end
    
    if PlayerCharacter.level >= MAX_LEVEL then
        log("Already at max level (" .. MAX_LEVEL .. ")!")
        return
    end
    
    PlayerCharacter.experience = PlayerCharacter.experience - PlayerCharacter.experienceToNextLevel
    PlayerCharacter.level = PlayerCharacter.level + 1
    PlayerCharacter.experienceToNextLevel = math.floor(PlayerCharacter.experienceToNextLevel * 1.15)
    
    log("=== LEVEL UP! ===")
    log("Now level " .. PlayerCharacter.level)
    
    -- Increase health
    local healthGain = PlayerCharacter.class.healthPerLevel
    PlayerCharacter.maxHealth = PlayerCharacter.maxHealth + healthGain
    PlayerCharacter.currentHealth = PlayerCharacter.maxHealth
    log("Max health increased by " .. healthGain .. " (now " .. PlayerCharacter.maxHealth .. ")")
    
    -- Increase resource
    local resourceGain = PlayerCharacter.class.resourcePerLevel
    PlayerCharacter.maxResource = PlayerCharacter.maxResource + resourceGain
    PlayerCharacter.currentResource = PlayerCharacter.maxResource
    log("Max " .. PlayerCharacter.class.resourceType .. " increased by " .. 
        resourceGain .. " (now " .. PlayerCharacter.maxResource .. ")")
    
    -- Grant stat points
    PlayerCharacter.availableStatPoints = PlayerCharacter.availableStatPoints + STAT_POINTS_PER_LEVEL
    log("Gained " .. STAT_POINTS_PER_LEVEL .. " stat points to distribute")
    
    -- Unlock new abilities (from abilities.lua)
    unlockAbilitiesForLevel(PlayerCharacter.level)
end

function unlockAbilitiesForLevel(level)
    if not PlayerCharacter.class then return end
    
    -- This would query abilities.lua to find abilities for this class at this level
    -- Simplified for plugin demonstration
    log("Checking for new abilities at level " .. level .. " (see abilities.lua)")
    
    -- Example: Mark ability as unlocked
    -- In practice, this would iterate through abilities from abilities.lua
    -- PlayerCharacter.unlockedAbilities[abilityName] = true
end

function increaseStat(statName, amount)
    if PlayerCharacter.availableStatPoints < amount then
        log("Not enough stat points!")
        return false
    end
    
    if not PlayerCharacter.stats[statName] then
        log("Invalid stat: " .. statName)
        return false
    end
    
    PlayerCharacter.stats[statName] = PlayerCharacter.stats[statName] + amount
    PlayerCharacter.availableStatPoints = PlayerCharacter.availableStatPoints - amount
    
    log(statName .. " increased to " .. PlayerCharacter.stats[statName])
    
    -- Apply stat effects
    if statName == "CON" then
        local healthBonus = amount * 5
        PlayerCharacter.maxHealth = PlayerCharacter.maxHealth + healthBonus
        log("Max health increased by " .. healthBonus)
    end
    
    return true
end

function displayCharacterSheet()
    if not PlayerCharacter.class then
        log("No class selected!")
        return
    end
    
    log("=== Character Sheet ===")
    log("Class: " .. PlayerCharacter.class.name)
    log("Level: " .. PlayerCharacter.level .. "/" .. MAX_LEVEL .. 
        " (XP: " .. PlayerCharacter.experience .. "/" .. PlayerCharacter.experienceToNextLevel .. ")")
    log("")
    
    if PlayerCharacter.isGhost then
        log("STATUS: GHOST FORM - All shapeshifts and buffs removed")
        log("Resurrect to return to normal")
    elseif PlayerCharacter.isDead then
        log("STATUS: DEAD")
    else
        log("Health: " .. math.floor(PlayerCharacter.currentHealth) .. "/" .. PlayerCharacter.maxHealth)
        log(PlayerCharacter.class.resourceType .. ": " .. 
            math.floor(PlayerCharacter.currentResource) .. "/" .. PlayerCharacter.maxResource)
        log("Combat Status: " .. (PlayerCharacter.inCombat and "IN COMBAT" or "Out of Combat"))
        
        if PlayerCharacter.currentShapeshift then
            log("Shapeshifted: " .. PlayerCharacter.currentShapeshift)
        end
    end
    
    log("")
    log("=== Stats ===")
    log("STR: " .. PlayerCharacter.stats.STR .. "  DEX: " .. PlayerCharacter.stats.DEX .. 
        "  CON: " .. PlayerCharacter.stats.CON)
    log("INT: " .. PlayerCharacter.stats.INT .. "  WIS: " .. PlayerCharacter.stats.WIS .. 
        "  CHA: " .. PlayerCharacter.stats.CHA)
    log("Available stat points: " .. PlayerCharacter.availableStatPoints)
    log("")
    log("=== Abilities ===")
    log("See abilities.lua for full ability list")
    local count = 0
    for name, unlocked in pairs(PlayerCharacter.unlockedAbilities) do
        if unlocked then
            count = count + 1
        end
    end
    log("Unlocked abilities: " .. count)
end

-- Death and Ghost Form mechanics
function takeDamage(amount)
    if PlayerCharacter.isDead or PlayerCharacter.isGhost then
        return
    end
    
    PlayerCharacter.currentHealth = PlayerCharacter.currentHealth - amount
    log("Took " .. amount .. " damage (" .. 
        math.floor(PlayerCharacter.currentHealth) .. "/" .. PlayerCharacter.maxHealth .. " HP)")
    
    if PlayerCharacter.currentHealth <= 0 then
        die()
    end
end

function die()
    PlayerCharacter.isDead = true
    PlayerCharacter.isGhost = true
    PlayerCharacter.currentHealth = 0
    
    -- Remove all shapeshifts (Druid specific)
    if PlayerCharacter.currentShapeshift then
        log("Shapeshift form removed due to death")
        PlayerCharacter.currentShapeshift = nil
        PlayerCharacter.shapeshiftedForm = nil
    end
    
    -- Remove all buffs unless they persist through death
    local persistentBuffs = {}
    for buffName, buff in pairs(PlayerCharacter.activeBuffs) do
        if buff.persistsThroughDeath then
            persistentBuffs[buffName] = buff
        end
    end
    PlayerCharacter.activeBuffs = persistentBuffs
    
    log("=== YOU HAVE DIED ===")
    log("You are now in ghost form")
    log("Shapeshifts and non-persistent buffs have been removed")
end

function resurrect()
    if not PlayerCharacter.isDead then
        log("Not dead!")
        return false
    end
    
    PlayerCharacter.isDead = false
    PlayerCharacter.isGhost = false
    PlayerCharacter.currentHealth = math.floor(PlayerCharacter.maxHealth * 0.5)  -- 50% health
    PlayerCharacter.currentResource = math.floor(PlayerCharacter.maxResource * 0.2)  -- 20% resource
    
    log("=== RESURRECTED ===")
    log("Health: " .. PlayerCharacter.currentHealth .. "/" .. PlayerCharacter.maxHealth)
    log("You can now fight again!")
    
    return true
end

-- Shapeshift management (Druid only)
function shapeshift(formName)
    if not PlayerCharacter.class then
        log("No class selected!")
        return false
    end
    
    if PlayerCharacter.class.name ~= "Druid" then
        log("Only Druids can shapeshift!")
        return false
    end
    
    if PlayerCharacter.isDead or PlayerCharacter.isGhost then
        log("Cannot shapeshift while dead!")
        return false
    end
    
    -- Would check shapeshift_forms.lua for form details
    PlayerCharacter.currentShapeshift = formName
    log("Shapeshifted into: " .. formName)
    log("Shapeshift lasts until you unshift or die")
    
    return true
end

function unshift()
    if not PlayerCharacter.currentShapeshift then
        log("Not shapeshifted!")
        return false
    end
    
    local previousForm = PlayerCharacter.currentShapeshift
    PlayerCharacter.currentShapeshift = nil
    PlayerCharacter.shapeshiftedForm = nil
    
    log("Unshifted from " .. previousForm .. " back to normal form")
    return true
end
