-- Ranger Class Stats
-- DEX-based ranged DPS with animal companion

return {
    name = "Ranger",
    description = "Master of ranged combat and nature. Rangers bond with animal companions and excel at tracking and survival.",
    
    primaryStat = "DEX",
    
    resourceType = "Focus",
    baseResource = 100,
    resourcePerLevel = 10,
    
    combatRegen = 2,
    outOfCombatRegen = 10,
    
    hitDieSize = 8,
    baseHealth = 55,
    healthPerLevel = 8,
    
    allowedWeapons = {"bow", "crossbow", "dagger", "sword", "axe"},
    allowedArmor = {"cloth", "leather", "mail"},
    canDualWield = true,
    canUseShield = false,
    
    startingStats = {
        STR = 12,
        DEX = 16,  -- Primary
        CON = 12,
        INT = 10,
        WIS = 14,
        CHA = 8
    },
    
    statGrowth = {
        STR = 1,
        DEX = 2,  -- Highest
        CON = 1,
        INT = 0.5,
        WIS = 1.5,
        CHA = 0
    },
    
    -- Pet system
    hasPets = true,
    maxPets = 1
}
