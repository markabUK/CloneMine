-- Wizard Class Stats
-- INT-based arcane spellcaster through study and research

return {
    name = "Wizard",
    description = "Masters of arcane magic through study and research. Wizards learn their spells from spellbooks and prepare them carefully.",
    
    -- Primary stat
    primaryStat = "INT",
    
    -- Resource system
    resourceType = "Mana",
    baseResource = 100,
    resourcePerLevel = 15,
    
    -- Combat regeneration (per second)
    combatRegen = 2,
    outOfCombatRegen = 10,
    
    -- Health system
    hitDieSize = 6,
    baseHealth = 40,
    healthPerLevel = 6,
    
    -- Equipment restrictions
    allowedWeapons = {"staff", "wand", "dagger"},
    allowedArmor = {"cloth"},
    canDualWield = false,
    canUseShield = false,
    
    -- Starting stats (D&D style)
    startingStats = {
        STR = 8,
        DEX = 10,
        CON = 10,
        INT = 16,  -- Primary stat
        WIS = 12,
        CHA = 10
    },
    
    -- Stat growth per level
    statGrowth = {
        STR = 0,
        DEX = 0.5,
        CON = 0.5,
        INT = 2,  -- Highest growth
        WIS = 1,
        CHA = 0.5
    }
}
