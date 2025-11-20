-- D&D-Style Character Classes Plugin
-- Implements class system with specializations

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
        },
        
        abilities = {
            {level = 1, name = "Magic Missile", cost = 10, damage = 15},
            {level = 3, name = "Fireball", cost = 25, damage = 40, aoe = true},
            {level = 5, name = "Ice Storm", cost = 35, damage = 50, aoe = true, slow = true},
            {level = 7, name = "Lightning Bolt", cost = 30, damage = 60, chain = true},
            {level = 10, name = "Meteor", cost = 60, damage = 100, aoe = true}
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
        },
        
        abilities = {
            {level = 1, name = "Chaos Bolt", cost = 12, damage = 20, random = true},
            {level = 3, name = "Wild Magic Surge", cost = 20, damage = 35, unpredictable = true},
            {level = 5, name = "Arcane Barrage", cost = 30, damage = 45, multiHit = true},
            {level = 7, name = "Time Stop", cost = 50, duration = 3, utility = true},
            {level = 10, name = "Wish", cost = 80, versatile = true}
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
        },
        
        abilities = {
            {level = 1, name = "Heal", cost = 15, healing = 30},
            {level = 3, name = "Holy Smite", cost = 20, damage = 35, undead = 2.0},
            {level = 5, name = "Divine Shield", cost = 25, shield = 50, duration = 10},
            {level = 7, name = "Mass Heal", cost = 40, healing = 40, aoe = true},
            {level = 10, name = "Resurrection", cost = 100, revive = true}
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
        },
        
        abilities = {
            {level = 1, name = "Power Strike", cost = 15, damage = 25, physicalBonus = 1.5},
            {level = 3, name = "Whirlwind", cost = 25, damage = 30, aoe = true},
            {level = 5, name = "Execute", cost = 20, damage = 60, lowHealthBonus = true},
            {level = 7, name = "Battle Cry", cost = 30, teamBuff = true, duration = 15},
            {level = 10, name = "Bladestorm", cost = 50, damage = 40, aoe = true, duration = 5}
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
        },
        
        abilities = {
            {level = 1, name = "Aimed Shot", cost = 10, damage = 25, accurate = true},
            {level = 2, name = "Call Pet", cost = 0, summonPet = true},
            {level = 3, name = "Multi-Shot", cost = 20, damage = 20, targets = 3},
            {level = 5, name = "Trap", cost = 15, damage = 35, crowd = true},
            {level = 7, name = "Camouflage", cost = 25, stealth = true, duration = 30},
            {level = 10, name = "Barrage", cost = 40, damage = 50, rapid = true}
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
        },
        
        abilities = {
            {level = 1, name = "Shadow Bolt", cost = 10, damage = 20, dot = 5},
            {level = 2, name = "Summon Demon", cost = 20, summonPet = true},
            {level = 3, name = "Curse of Weakness", cost = 15, debuff = true, duration = 20},
            {level = 5, name = "Drain Life", cost = 20, damage = 25, lifesteal = 0.5},
            {level = 7, name = "Fear", cost = 25, crowd = true, duration = 8},
            {level = 10, name = "Soul Siphon", cost = 50, damage = 70, resourceDrain = true}
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
        },
        
        abilities = {
            {level = 1, name = "Wrath", cost = 10, damage = 18, nature = true},
            {level = 2, name = "Bear Form", cost = 20, tank = true, duration = 60},
            {level = 3, name = "Healing Touch", cost = 15, healing = 35},
            {level = 5, name = "Moonfire", cost = 20, damage = 25, dot = 10, duration = 12},
            {level = 7, name = "Cat Form", cost = 20, dps = true, duration = 60},
            {level = 10, name = "Starfall", cost = 50, damage = 60, aoe = true, duration = 10}
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
        
        abilities = {
            {level = 1, name = "Summon Minion", cost = 20, summonPet = "minion"},
            {level = 1, name = "Soothe", cost = 10, healing = 15, hot = true},
            {level = 3, name = "Fortify", cost = 20, buff = "defense", duration = 60},
            {level = 5, name = "Rally", cost = 30, healing = 25, aoe = true, buff = "speed"},
            {level = 7, name = "Empower Pets", cost = 25, petDamageBonus = 2.0, duration = 15},
            {level = 10, name = "Summon Lieutenant", cost = 40, summonPet = "lieutenant"},
            {level = 15, name = "Mass Fortification", cost = 50, buff = "allStats", aoe = true},
            {level = 20, name = "Summon Boss Pet", cost = 60, summonPet = "boss"}
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
    
    -- Available stat points
    availableStatPoints = 0,
    
    -- Unlocked abilities
    unlockedAbilities = {}
}

function onLoad()
    log("Character Classes Plugin loaded!")
    log("Available classes:")
    for className, class in pairs(CharacterClass) do
        log("  - " .. class.name .. " (" .. class.description .. ")")
        log("    Primary: " .. class.primaryStat .. " | Resource: " .. class.resourceType)
    end
end

function onUpdate(deltaTime)
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
    
    -- Unlock level 1 abilities
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
    
    -- Find ability
    local ability = nil
    for _, ab in ipairs(PlayerCharacter.class.abilities) do
        if ab.name == abilityName then
            ability = ab
            break
        end
    end
    
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

function addExperience(amount)
    PlayerCharacter.experience = PlayerCharacter.experience + amount
    log("Gained " .. amount .. " experience")
    
    while PlayerCharacter.experience >= PlayerCharacter.experienceToNextLevel do
        levelUp()
    end
end

function levelUp()
    if not PlayerCharacter.class then return end
    
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
    PlayerCharacter.availableStatPoints = PlayerCharacter.availableStatPoints + 3
    log("Gained 3 stat points to distribute")
    
    -- Unlock new abilities
    unlockAbilitiesForLevel(PlayerCharacter.level)
end

function unlockAbilitiesForLevel(level)
    if not PlayerCharacter.class then return end
    
    for _, ability in ipairs(PlayerCharacter.class.abilities) do
        if ability.level == level and not PlayerCharacter.unlockedAbilities[ability.name] then
            PlayerCharacter.unlockedAbilities[ability.name] = true
            log("Unlocked ability: " .. ability.name .. " (cost: " .. ability.cost .. ")")
        end
    end
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
    log("Level: " .. PlayerCharacter.level .. " (XP: " .. PlayerCharacter.experience .. 
        "/" .. PlayerCharacter.experienceToNextLevel .. ")")
    log("")
    log("Health: " .. math.floor(PlayerCharacter.currentHealth) .. "/" .. PlayerCharacter.maxHealth)
    log(PlayerCharacter.class.resourceType .. ": " .. 
        math.floor(PlayerCharacter.currentResource) .. "/" .. PlayerCharacter.maxResource)
    log("Combat Status: " .. (PlayerCharacter.inCombat and "IN COMBAT" or "Out of Combat"))
    log("")
    log("=== Stats ===")
    log("STR: " .. PlayerCharacter.stats.STR .. "  DEX: " .. PlayerCharacter.stats.DEX .. 
        "  CON: " .. PlayerCharacter.stats.CON)
    log("INT: " .. PlayerCharacter.stats.INT .. "  WIS: " .. PlayerCharacter.stats.WIS .. 
        "  CHA: " .. PlayerCharacter.stats.CHA)
    log("Available stat points: " .. PlayerCharacter.availableStatPoints)
    log("")
    log("=== Abilities ===")
    for name, unlocked in pairs(PlayerCharacter.unlockedAbilities) do
        if unlocked then
            for _, ab in ipairs(PlayerCharacter.class.abilities) do
                if ab.name == name then
                    log("  " .. name .. " (cost: " .. ab.cost .. ")")
                    break
                end
            end
        end
    end
end
