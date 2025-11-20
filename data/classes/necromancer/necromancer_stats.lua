-- Necromancer Class Stats
-- Death magic specialist with undead summoning

return {
    name = "Necromancer",
    description = "Master of death magic who commands an army of undead minions. Drains life from enemies and spreads disease.",
    
    -- Primary stat and resources
    primaryStat = "INT",
    resourceType = "Mana",
    resourceName = "Mana",
    
    -- Base stats at level 1
    baseHealth = 80,
    baseResource = 150,
    baseArmor = 5,
    
    -- Stat scaling per level
    healthPerLevel = 8,
    resourcePerLevel = 12,
    armorPerLevel = 0.5,
    
    -- Resource regeneration (per second)
    regenInCombat = 2,
    regenOutOfCombat = 10,
    
    -- Equipment restrictions
    allowedArmor = {"CLOTH"},
    allowedWeapons = {"WAND", "STAFF", "DAGGER"},
    canDualWield = false,
    canUseShield = false,
    
    -- Starting stats (total = 75)
    startingStats = {
        STR = 8,   -- Low physical strength
        DEX = 10,  -- Average dexterity
        CON = 12,  -- Slightly above average constitution
        INT = 20,  -- High intelligence (primary)
        WIS = 15,  -- Good wisdom for spell defense
        CHA = 10   -- Average charisma
    },
    
    -- Stat priorities for auto-allocation
    statPriorities = {"INT", "CON", "WIS", "DEX", "CHA", "STR"},
    
    -- Class features
    features = {
        "Undead Summoning",
        "Life Drain",
        "Disease and Decay",
        "Death Magic",
        "Corpse Manipulation",
        "Fear Effects"
    },
    
    -- Role classification
    role = "Caster DPS",
    subRole = "Summoner",
    difficulty = "Medium",
    
    -- Level cap
    maxLevel = 20,
    expandedMaxLevel = 120  -- Future expansion
}
