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
    canShapeshift = true,
    shapeshiftFile = "druid_shapeshift",  -- References druid_shapeshift.lua in same directory
    
    -- Defensive mechanics this class can use
    defensiveMechanics = {
        canDodge = true,
        canParry = false,
        canBlock = false  -- Druid cannot block
    },
    
    -- Class-specific damage scaling overrides
    damageScaling = {
        damageDealt = {
            WIS = {
                spellDamage = 0.045,         -- +4.5% spell damage per WIS (higher for nature spells)
                healingDone = 0.05,          -- +5% healing per WIS (best healer in forms)
                shapeshiftPower = 0.02       -- +2% shapeshift power per WIS
            },
            STR = {
                shapeshiftDamage = 0.025,    -- +2.5% physical damage in animal forms per STR
                bearFormArmor = 0.08         -- +0.08 armor per STR in bear form
            },
            DEX = {
                catFormDamage = 0.03,        -- +3% damage in cat form per DEX
                dodgeInForms = 0.15          -- +0.15% dodge in agile forms per DEX
            }
        },
        damageTaken = {
            CON = {
                physicalReduction = 0.065,   -- +6.5% physical reduction per CON
                bearFormReduction = 0.10     -- +10% in bear form per CON (tank form)
            },
            WIS = {
                magicResist = 0.04           -- +4% magic resist per WIS
            }
        },
        levelScaling = {
            shapeshiftPowerPerLevel = 0.04,  -- +4% shapeshift power per level
            healthPerLevel = 0.05            -- +5% health per level
        }
    }
}
