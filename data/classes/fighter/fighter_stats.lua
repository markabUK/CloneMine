-- Fighter Class Stats
return {
    name = "Fighter",
    description = "Master of weapons and combat, the ultimate warrior",
    primaryStat = "STR",
    resourceType = "Energy",
    baseResource = 100,
    resourcePerLevel = 10,
    combatRegen = 2,
    outOfCombatRegen = 10,
    hitDieSize = 10,
    baseHealth = 70,
    healthPerLevel = 10,
    allowedWeapons = {"sword", "axe", "mace", "spear", "bow", "crossbow"},
    allowedArmor = {"cloth", "leather", "mail", "plate"},
    canDualWield = true,
    canUseShield = true,
    startingStats = {STR = 16, DEX = 12, CON = 14, INT = 8, WIS = 10, CHA = 8},
    statGrowth = {STR = 2, DEX = 1, CON = 1.5, INT = 0, WIS = 0.5, CHA = 0},
    
    -- Damage scaling overrides
    damageScalingOverrides = {
        damageDealt = {
            STR = {physicalDamage = 0.045},  -- Fighters get 4.5% per STR (higher)
            DEX = {physicalDamage = 0.015}
        },
        damageTaken = {
            STR = {blockChance = 0.12},  -- Higher block chance
            CON = {physicalReduction = 0.08}  -- Best CON scaling
        }
    }
}
