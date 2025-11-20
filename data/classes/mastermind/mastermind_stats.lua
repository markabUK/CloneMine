-- Mastermind Class Stats
return {
    name = "Mastermind",
    description = "Pet controller from City of Villains. All damage through pets, supports group",
    primaryStat = "CHA",
    resourceType = "Control",
    baseResource = 100,
    resourcePerLevel = 12,
    combatRegen = 2,
    outOfCombatRegen = 10,
    hitDieSize = 6,
    baseHealth = 42,
    healthPerLevel = 6,
    allowedWeapons = {"wand", "staff", "dagger"},
    allowedArmor = {"cloth", "leather"},
    canDualWield = false,
    canUseShield = false,
    startingStats = {STR = 8, DEX = 10, CON = 10, INT = 12, WIS = 10, CHA = 16},
    statGrowth = {STR = 0, DEX = 0.5, CON = 0.5, INT = 1, WIS = 1, CHA = 2},
    hasPets = true,
    maxPets = 6
}
