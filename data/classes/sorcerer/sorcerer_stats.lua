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
    statGrowth = {STR = 0, DEX = 0.5, CON = 0.5, INT = 1, WIS = 0.5, CHA = 2},
    
    -- Defensive mechanics this class can use
    defensiveMechanics = {
        canDodge = true,
        canParry = false,
        canBlock = false
    },
    
    -- Class-specific damage scaling
    damageScaling = {
        damageDealt = {
            CHA = {
                spellDamage = 0.055,         -- +5.5% spell damage per CHA (highest caster scaling)
                critChance = 0.8,            -- +0.8% crit chance per CHA
                critDamage = 0.015           -- +1.5% crit damage per CHA
            },
            INT = {
                spellDamage = 0.02           -- +2% spell damage per INT
            }
        },
        damageTaken = {
            CON = {
                physicalReduction = 0.04     -- +4% physical reduction per CON
            },
            CHA = {
                magicReduction = 0.06        -- +6% magic reduction per CHA
            }
        },
        levelScaling = {
            spellDamagePerLevel = 0.07,      -- +7% spell damage per level (highest growth)
            healthPerLevel = 0.04            -- +4% health per level
        }
    }
}
