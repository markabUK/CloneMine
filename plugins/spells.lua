-- Spell System Plugin
-- Defines all spells and their class associations

-- Spell School categories
local SpellSchool = {
    ARCANE = "Arcane",
    FIRE = "Fire",
    FROST = "Frost",
    NATURE = "Nature",
    SHADOW = "Shadow",
    HOLY = "Holy",
    PHYSICAL = "Physical"
}

-- Spell definitions
local Spells = {
    -- Arcane Spells
    MAGIC_MISSILE = {
        name = "Magic Missile",
        school = SpellSchool.ARCANE,
        cost = 10,
        damage = 15,
        castTime = 1.5,
        cooldown = 0,
        requiredLevel = 1,
        description = "Launches magical projectiles that always hit",
        classes = {"WIZARD"}
    },
    
    ARCANE_BARRAGE = {
        name = "Arcane Barrage",
        school = SpellSchool.ARCANE,
        cost = 30,
        damage = 45,
        castTime = 2.0,
        cooldown = 8,
        requiredLevel = 5,
        description = "Rapid fire arcane missiles",
        multiHit = 3,
        classes = {"WIZARD", "SORCERER"}
    },
    
    ARCANE_INTELLECT = {
        name = "Arcane Intellect",
        school = SpellSchool.ARCANE,
        cost = 20,
        castTime = 0,
        cooldown = 0,
        requiredLevel = 2,
        description = "Increases INT by 10% for 1 hour",
        buff = {stat = "INT", multiplier = 1.1, duration = 3600},
        classes = {"WIZARD", "SORCERER"}
    },
    
    -- Fire Spells
    FIREBALL = {
        name = "Fireball",
        school = SpellSchool.FIRE,
        cost = 25,
        damage = 40,
        castTime = 2.5,
        cooldown = 0,
        requiredLevel = 3,
        description = "Hurls a fiery ball that explodes on impact",
        aoe = true,
        radius = 5,
        classes = {"WIZARD", "SORCERER"}  -- Shared spell!
    },
    
    FIRE_BLAST = {
        name = "Fire Blast",
        school = SpellSchool.FIRE,
        cost = 15,
        damage = 25,
        castTime = 1.5,
        cooldown = 0,
        requiredLevel = 1,
        description = "Quick fire damage",
        classes = {"WIZARD", "SORCERER", "WARLOCK"}
    },
    
    METEOR = {
        name = "Meteor",
        school = SpellSchool.FIRE,
        cost = 60,
        damage = 100,
        castTime = 4.0,
        cooldown = 30,
        requiredLevel = 10,
        description = "Summons a meteor from the sky",
        aoe = true,
        radius = 10,
        classes = {"WIZARD"}
    },
    
    -- Frost Spells
    FROSTBOLT = {
        name = "Frostbolt",
        school = SpellSchool.FROST,
        cost = 12,
        damage = 20,
        castTime = 2.0,
        cooldown = 0,
        requiredLevel = 1,
        description = "Launches a bolt of frost",
        slow = {amount = 0.5, duration = 4},
        classes = {"WIZARD"}
    },
    
    ICE_STORM = {
        name = "Ice Storm",
        school = SpellSchool.FROST,
        cost = 35,
        damage = 50,
        castTime = 3.0,
        cooldown = 12,
        requiredLevel = 5,
        description = "Summons a storm of ice",
        aoe = true,
        radius = 8,
        slow = {amount = 0.7, duration = 6},
        classes = {"WIZARD"}
    },
    
    BLIZZARD = {
        name = "Blizzard",
        school = SpellSchool.FROST,
        cost = 50,
        damage = 30,
        castTime = 0,
        channeled = true,
        channelDuration = 8,
        cooldown = 60,
        requiredLevel = 8,
        description = "Channels a massive blizzard over an area",
        aoe = true,
        radius = 15,
        damagePerTick = 10,
        tickInterval = 1,
        classes = {"WIZARD"}
    },
    
    -- Shadow Spells
    SHADOW_BOLT = {
        name = "Shadow Bolt",
        school = SpellSchool.SHADOW,
        cost = 10,
        damage = 20,
        castTime = 2.0,
        cooldown = 0,
        requiredLevel = 1,
        description = "Sends a bolt of shadow energy",
        dot = {damage = 5, duration = 12, tickInterval = 3},
        classes = {"WARLOCK"}
    },
    
    DRAIN_LIFE = {
        name = "Drain Life",
        school = SpellSchool.SHADOW,
        cost = 20,
        damage = 25,
        castTime = 0,
        channeled = true,
        channelDuration = 5,
        cooldown = 0,
        requiredLevel = 5,
        description = "Drains life from target and heals caster",
        lifesteal = 0.5,
        classes = {"WARLOCK"}
    },
    
    CURSE_OF_AGONY = {
        name = "Curse of Agony",
        school = SpellSchool.SHADOW,
        cost = 15,
        damage = 0,
        castTime = 0,
        cooldown = 0,
        requiredLevel = 2,
        description = "Curses target with increasing pain",
        dot = {damage = 40, duration = 24, tickInterval = 3, rampUp = true},
        classes = {"WARLOCK"}
    },
    
    FEAR = {
        name = "Fear",
        school = SpellSchool.SHADOW,
        cost = 25,
        damage = 0,
        castTime = 1.5,
        cooldown = 30,
        requiredLevel = 7,
        description = "Strikes fear into enemy's heart",
        crowdControl = {type = "fear", duration = 8},
        classes = {"WARLOCK", "PRIEST"}
    },
    
    -- Holy Spells
    HEAL = {
        name = "Heal",
        school = SpellSchool.HOLY,
        cost = 15,
        healing = 30,
        castTime = 2.5,
        cooldown = 0,
        requiredLevel = 1,
        description = "Heals a friendly target",
        classes = {"PRIEST", "DRUID"}
    },
    
    FLASH_HEAL = {
        name = "Flash Heal",
        school = SpellSchool.HOLY,
        cost = 20,
        healing = 20,
        castTime = 1.5,
        cooldown = 0,
        requiredLevel = 3,
        description = "Quick healing spell",
        classes = {"PRIEST"}
    },
    
    HOLY_SMITE = {
        name = "Holy Smite",
        school = SpellSchool.HOLY,
        cost = 20,
        damage = 35,
        castTime = 2.0,
        cooldown = 8,
        requiredLevel = 3,
        description = "Smites enemy with holy power",
        bonusAgainstUndead = 2.0,
        classes = {"PRIEST"}
    },
    
    DIVINE_SHIELD = {
        name = "Divine Shield",
        school = SpellSchool.HOLY,
        cost = 25,
        castTime = 0,
        cooldown = 300,
        requiredLevel = 5,
        description = "Makes caster immune to damage",
        shield = {amount = 9999, duration = 8},
        classes = {"PRIEST"}
    },
    
    MASS_HEAL = {
        name = "Mass Heal",
        school = SpellSchool.HOLY,
        cost = 40,
        healing = 40,
        castTime = 3.0,
        cooldown = 10,
        requiredLevel = 7,
        description = "Heals all nearby allies",
        aoe = true,
        radius = 15,
        classes = {"PRIEST"}
    },
    
    RESURRECTION = {
        name = "Resurrection",
        school = SpellSchool.HOLY,
        cost = 100,
        castTime = 10.0,
        cooldown = 600,
        requiredLevel = 10,
        description = "Brings a dead ally back to life",
        revive = true,
        classes = {"PRIEST", "DRUID"}
    },
    
    -- Nature Spells
    WRATH = {
        name = "Wrath",
        school = SpellSchool.NATURE,
        cost = 10,
        damage = 18,
        castTime = 2.0,
        cooldown = 0,
        requiredLevel = 1,
        description = "Blasts enemy with natural energy",
        classes = {"DRUID"}
    },
    
    MOONFIRE = {
        name = "Moonfire",
        school = SpellSchool.NATURE,
        cost = 20,
        damage = 25,
        castTime = 0,
        cooldown = 0,
        requiredLevel = 5,
        description = "Burns enemy with moonlight",
        dot = {damage = 10, duration = 12, tickInterval = 3},
        classes = {"DRUID"}
    },
    
    STARFALL = {
        name = "Starfall",
        school = SpellSchool.NATURE,
        cost = 50,
        damage = 60,
        castTime = 0,
        cooldown = 90,
        requiredLevel = 10,
        description = "Rains stars from the sky",
        aoe = true,
        radius = 20,
        duration = 10,
        damagePerTick = 15,
        tickInterval = 2,
        classes = {"DRUID"}
    },
    
    HEALING_TOUCH = {
        name = "Healing Touch",
        school = SpellSchool.NATURE,
        cost = 15,
        healing = 35,
        castTime = 3.0,
        cooldown = 0,
        requiredLevel = 3,
        description = "Heals target with nature's power",
        classes = {"DRUID"}
    },
    
    ENTANGLING_ROOTS = {
        name = "Entangling Roots",
        school = SpellSchool.NATURE,
        cost = 20,
        damage = 0,
        castTime = 1.5,
        cooldown = 20,
        requiredLevel = 4,
        description = "Roots enemy in place",
        crowdControl = {type = "root", duration = 10},
        classes = {"DRUID"}
    },
    
    -- Physical/Energy Spells
    POWER_STRIKE = {
        name = "Power Strike",
        school = SpellSchool.PHYSICAL,
        cost = 15,
        damage = 25,
        castTime = 0,
        cooldown = 6,
        requiredLevel = 1,
        description = "Powerful melee attack",
        physicalBonus = 1.5,
        classes = {"FIGHTER"}
    },
    
    WHIRLWIND = {
        name = "Whirlwind",
        school = SpellSchool.PHYSICAL,
        cost = 25,
        damage = 30,
        castTime = 0,
        cooldown = 10,
        requiredLevel = 3,
        description = "Spin attack hitting all nearby enemies",
        aoe = true,
        radius = 5,
        classes = {"FIGHTER"}
    },
    
    EXECUTE = {
        name = "Execute",
        school = SpellSchool.PHYSICAL,
        cost = 20,
        damage = 60,
        castTime = 0,
        cooldown = 15,
        requiredLevel = 5,
        description = "Finishing move, bonus damage below 20% health",
        lowHealthBonus = 2.0,
        classes = {"FIGHTER"}
    },
    
    BATTLE_CRY = {
        name = "Battle Cry",
        school = SpellSchool.PHYSICAL,
        cost = 30,
        castTime = 0,
        cooldown = 120,
        requiredLevel = 7,
        description = "Increases party attack power",
        buff = {stat = "attackPower", amount = 50, duration = 15, aoe = true},
        classes = {"FIGHTER"}
    },
    
    -- Ranger Spells
    AIMED_SHOT = {
        name = "Aimed Shot",
        school = SpellSchool.PHYSICAL,
        cost = 10,
        damage = 25,
        castTime = 2.5,
        cooldown = 0,
        requiredLevel = 1,
        description = "Carefully aimed shot",
        accurate = true,
        bonusCrit = 0.25,
        classes = {"RANGER"}
    },
    
    MULTI_SHOT = {
        name = "Multi-Shot",
        school = SpellSchool.PHYSICAL,
        cost = 20,
        damage = 20,
        castTime = 0,
        cooldown = 10,
        requiredLevel = 3,
        description = "Shoots multiple targets",
        targets = 3,
        classes = {"RANGER"}
    },
    
    TRAP = {
        name = "Frost Trap",
        school = SpellSchool.FROST,
        cost = 15,
        damage = 35,
        castTime = 0,
        cooldown = 30,
        requiredLevel = 5,
        description = "Sets a trap that freezes enemies",
        crowdControl = {type = "freeze", duration = 5},
        classes = {"RANGER"}
    },
    
    CAMOUFLAGE = {
        name = "Camouflage",
        school = SpellSchool.NATURE,
        cost = 25,
        castTime = 0,
        cooldown = 60,
        requiredLevel = 7,
        description = "Become invisible",
        stealth = {duration = 30},
        classes = {"RANGER"}
    },
    
    -- Mastermind Support Spells
    SOOTHE = {
        name = "Soothe",
        school = SpellSchool.HOLY,
        cost = 10,
        healing = 15,
        castTime = 1.5,
        cooldown = 0,
        requiredLevel = 1,
        description = "Gentle healing over time",
        hot = {healing = 20, duration = 12, tickInterval = 3},
        classes = {"MASTERMIND"}
    },
    
    FORTIFY = {
        name = "Fortify",
        school = SpellSchool.ARCANE,
        cost = 20,
        castTime = 0,
        cooldown = 30,
        requiredLevel = 3,
        description = "Increases ally's defense",
        buff = {stat = "defense", amount = 30, duration = 60},
        classes = {"MASTERMIND"}
    },
    
    RALLY = {
        name = "Rally",
        school = SpellSchool.HOLY,
        cost = 30,
        healing = 25,
        castTime = 0,
        cooldown = 60,
        requiredLevel = 5,
        description = "Heals and energizes all allies",
        aoe = true,
        radius = 20,
        buff = {stat = "speed", multiplier = 1.2, duration = 10},
        classes = {"MASTERMIND"}
    },
    
    EMPOWER_PETS = {
        name = "Empower Pets",
        school = SpellSchool.ARCANE,
        cost = 25,
        castTime = 0,
        cooldown = 45,
        requiredLevel = 7,
        description = "Greatly increases pet damage",
        buff = {stat = "petDamage", multiplier = 2.0, duration = 15},
        classes = {"MASTERMIND"}
    }
}

-- Get spells available for a class
function getSpellsForClass(className)
    local classSpells = {}
    for spellId, spell in pairs(Spells) do
        for _, class in ipairs(spell.classes) do
            if class == className then
                table.insert(classSpells, {id = spellId, spell = spell})
                break
            end
        end
    end
    return classSpells
end

-- Get spell by name
function getSpell(spellName)
    return Spells[spellName]
end

-- Check if class can use spell
function canClassUseSpell(className, spellName)
    local spell = Spells[spellName]
    if not spell then return false end
    
    for _, class in ipairs(spell.classes) do
        if class == className then
            return true
        end
    end
    return false
end

function onLoad()
    log("Spell System Plugin loaded!")
    
    local totalSpells = 0
    local spellsBySchool = {}
    
    for _, spell in pairs(Spells) do
        totalSpells = totalSpells + 1
        spellsBySchool[spell.school] = (spellsBySchool[spell.school] or 0) + 1
    end
    
    log("Total spells: " .. totalSpells)
    log("Spells by school:")
    for school, count in pairs(spellsBySchool) do
        log("  " .. school .. ": " .. count)
    end
    
    -- Show shared spells
    log("\nShared spells (multiple classes):")
    for spellId, spell in pairs(Spells) do
        if #spell.classes > 1 then
            log("  " .. spell.name .. " - " .. table.concat(spell.classes, ", "))
        end
    end
end

-- Export for other plugins
_G.Spells = Spells
_G.getSpellsForClass = getSpellsForClass
_G.getSpell = getSpell
_G.canClassUseSpell = canClassUseSpell
