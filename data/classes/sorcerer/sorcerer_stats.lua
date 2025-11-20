-- Sorcerer Class Stats
return {
    name = "Sorcerer",
    description = "Innate spellcasters powered by raw charisma and natural magic",
    primaryStat = "CHA",
    resourceType = "Mana",
    baseResource = 120,
    resourcePerLevel = 18,
    combatRegen = 2,
    outOfCombatRegen = 10,
    hitDieSize = 6,
    baseHealth = 45,
    healthPerLevel = 6,
    allowedWeapons = {"staff", "wand", "dagger"},
    allowedArmor = {"cloth"},
    canDualWield = false,
    canUseShield = false,
    startingStats = {STR = 8, DEX = 10, CON = 11, INT = 10, WIS = 10, CHA = 16},
    statGrowth = {STR = 0, DEX = 0.5, CON = 0.5, INT = 1, WIS = 0.5, CHA = 2}
}
