-- Player Spells (Separate from Monster Spells)
-- Spells usable by player characters only
-- This file contains spells from all character classes

-- Note: This is a consolidated list. Individual class spells are stored
-- in data/classes/{classname}/{classname}_spells.lua
-- This file is for reference and cross-class spell lookups

return {
    -- Shared spells between classes are listed here
    -- Individual class spell files should be consulted for complete spell lists
    
    -- Example: Fireball (Wizard/Sorcerer)
    {
        name = "Fireball",
        school = "Fire",
        classes = {"WIZARD", "SORCERER"},
        level = 3,
        manaCost = 35,
        castTime = 2.0,
        cooldown = 8,
        range = 35,
        damage = 50,
        damageType = "Fire",
        aoe = {radius = 6},
        description = "Hurls a ball of fire that explodes on impact."
    },
    
    -- Example: Heal (Priest/Druid)
    {
        name = "Heal",
        school = "Holy",
        classes = {"PRIEST", "DRUID"},
        level = 1,
        manaCost = 25,
        castTime = 2.5,
        cooldown = 0,
        range = 40,
        healing = 60,
        description = "Heals a friendly target."
    },
    
    -- Example: Fear (Warlock/Priest)
    {
        name = "Fear",
        school = "Shadow",
        classes = {"WARLOCK", "PRIEST"},
        level = 8,
        manaCost = 30,
        castTime = 1.5,
        cooldown = 20,
        range = 30,
        duration = 4,
        crowdControl = "Fear",
        description = "Strikes fear into the target, causing them to flee."
    }
    
    -- For complete spell lists, see individual class files:
    -- data/classes/wizard/wizard_spells.lua
    -- data/classes/sorcerer/sorcerer_spells.lua
    -- data/classes/priest/priest_spells.lua
    -- data/classes/fighter/fighter_spells.lua (if any)
    -- data/classes/ranger/ranger_spells.lua
    -- data/classes/warlock/warlock_spells.lua
    -- data/classes/druid/druid_spells.lua
    -- data/classes/mastermind/mastermind_spells.lua (if any)
    -- data/classes/necromancer/necromancer_spells.lua
}
