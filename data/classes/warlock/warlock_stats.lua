-- Warlock Class Stats
return {
    name = "Warlock",
    description = "Shadow magic wielders who make pacts with demons",
    primaryStat = "CHA",
    resourceType = "Mana",
    baseResource = 100,
    resourcePerLevel = 12,
    combatRegen = 2,
    outOfCombatRegen = 10,
    hitDieSize = 7,
    baseHealth = 48,
    healthPerLevel = 7,
    allowedWeapons = {"staff", "wand", "dagger"},
    allowedArmor = {"cloth", "leather"},
    canDualWield = false,
    canUseShield = false,
    startingStats = {STR = 8, DEX = 10, CON = 11, INT = 12, WIS = 8, CHA = 16},
    statGrowth = {STR = 0, DEX = 0.5, CON = 1, INT = 1.5, WIS = 0, CHA = 2},
    hasPets = true,
    maxPets = 1,
    
    -- Defensive mechanics (references data/scaling/stat_damage_system.lua)
    defensiveMechanics = {
        canDodge = true,
        canParry = false,
        canBlock = false
    },
    
    -- Damage scaling overrides (optional - overrides defaults in data/scaling/stat_damage_system.lua)
    damageScalingOverrides = {
        damageDealt = {
            CHA = {spellDamage = 0.010, petDamage = 0.012},  -- CHA scales both spells and pets
            INT = {spellDamage = 0.008}  -- Some INT scaling too
        },
        damageTaken = {
            CON = {physicalReduction = 0.008}
        }
    }
}
