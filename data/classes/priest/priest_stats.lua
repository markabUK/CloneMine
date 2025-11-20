-- Priest Class Stats
return {
    name = "Priest",
    description = "Divine healers and supporters who wield holy magic",
    primaryStat = "WIS",
    resourceType = "Mana",
    baseResource = 90,
    resourcePerLevel = 12,
    combatRegen = 2,
    outOfCombatRegen = 10,
    hitDieSize = 8,
    baseHealth = 50,
    healthPerLevel = 8,
    allowedWeapons = {"mace", "staff", "wand"},
    allowedArmor = {"cloth", "leather", "mail"},
    canDualWield = false,
    canUseShield = true,
    startingStats = {STR = 10, DEX = 8, CON = 12, INT = 10, WIS = 16, CHA = 12},
    statGrowth = {STR = 0.5, DEX = 0, CON = 1, INT = 1, WIS = 2, CHA = 1}
}
