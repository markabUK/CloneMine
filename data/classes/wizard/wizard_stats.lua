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
    },
    
    -- Defensive mechanics this class can use
    defensiveMechanics = {
        canDodge = true,
        canParry = false,
        canBlock = false
    },
    
    -- Class-specific damage scaling overrides
    damageScaling = {
        -- Damage dealt scaling
        damageDealt = {
            INT = {
                spellDamage = 0.05,          -- +5% spell damage per INT (higher than default 4%)
                spellPenetration = 0.8,      -- +0.8 spell penetration per INT
                dotDamage = 0.03             -- +3% DOT damage per INT
            },
            WIS = {
                spellDamage = 0.02,          -- +2% spell damage per WIS
                manaCost = 0.4               -- +0.4% mana cost reduction per WIS
            }
        },
        -- Damage taken reduction
        damageTaken = {
            CON = {
                physicalReduction = 0.04     -- +4% physical reduction per CON (slightly lower than default)
            },
            INT = {
                magicReduction = 0.08        -- +8% magic damage reduction per INT (wizards resist magic well)
            }
        },
        -- Level-based scaling
        levelScaling = {
            spellDamagePerLevel = 0.06,      -- +6% spell damage per level
            healthPerLevel = 0.04            -- +4% health per level
        }
    }
}
