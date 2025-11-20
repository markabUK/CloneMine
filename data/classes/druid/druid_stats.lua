-- Druid Class Stats
return {
    name = "Druid",
    description = "Nature magic wielders with unique shapeshifting abilities",
    primaryStat = "WIS",
    resourceType = "Mana",
    baseResource = 95,
    resourcePerLevel = 13,
    combatRegen = 2,
    outOfCombatRegen = 10,
    hitDieSize = 8,
    baseHealth = 53,
    healthPerLevel = 8,
    allowedWeapons = {"staff", "mace", "dagger"},
    allowedArmor = {"cloth", "leather"},
    canDualWield = false,
    canUseShield = false,
    startingStats = {STR = 10, DEX = 10, CON = 12, INT = 10, WIS = 16, CHA = 12},
    statGrowth = {STR = 1, DEX = 1, CON = 1, INT = 1, WIS = 2, CHA = 0.5},
    canShapeshift = true
}
