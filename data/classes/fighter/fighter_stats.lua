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
    
    -- Defensive mechanics this class can use
    defensiveMechanics = {
        canDodge = true,
        canParry = true,
        canBlock = true  -- Fighter can use all defensive types
    },
    
    -- Class-specific damage scaling overrides
    damageScaling = {
        damageDealt = {
            STR = {
                physicalDamage = 0.045,      -- +4.5% physical damage per STR (higher than default)
                criticalDamage = 0.015,      -- +1.5% crit damage per STR
                armorPenetration = 0.7       -- +0.7 armor penetration per STR
            },
            DEX = {
                physicalDamage = 0.015,      -- +1.5% from DEX
                criticalChance = 0.12,       -- +0.12% crit chance per DEX
                attackSpeed = 0.06           -- +0.06% attack speed per DEX
            }
        },
        damageTaken = {
            STR = {
                blockChance = 0.12,          -- +0.12% block chance per STR (with shield)
                physicalReduction = 0.03     -- +3% physical reduction per STR
            },
            CON = {
                physicalReduction = 0.08,    -- +8% physical reduction per CON (best of all classes)
                maxHealthIncrease = 0.03     -- +3% max health per CON
            }
        },
        levelScaling = {
            physicalDamagePerLevel = 0.05,   -- +5% physical damage per level
            healthPerLevel = 0.08            -- +8% health per level (highest)
        }
    }
}
