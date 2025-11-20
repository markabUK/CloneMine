-- Damage Scaling System
-- Configurable scaling for all entity types: Players, Pets, Monsters, Minions

return {
    -- ========================================
    -- PLAYER DAMAGE SCALING
    -- ========================================
    player = {
        baseLevelScaling = 0.05,
        baseStatScaling = 0.02,
        
        classScaling = {
            MASTERMIND = {
                levelScaling = 0.03,
                statScaling = 0.015,
                spellDamageMultiplier = 0.6,
                physicalDamageMultiplier = 0.5,
                petDamageMultiplier = 1.8,
                bossPetMultiplier = 2.5     -- Boss pet gets EXTRA 2.5x scaling
            },
            RANGER = {petDamageMultiplier = 1.0},
            WARLOCK = {petDamageMultiplier = 1.1},
            NECROMANCER = {petDamageMultiplier = 1.3}
        }
    },
    
    -- ========================================
    -- PET DAMAGE SCALING
    -- ========================================
    pet = {
        classPetScaling = {
            MASTERMIND = {
                petDamageMultiplier = 1.5,
                petHealthMultiplier = 1.2,
                ownerStatScaling = 0.7,
                petCountPenalty = 0.92,
                
                -- Tier-specific scaling
                tierScaling = {
                    [1] = {damageMultiplier = 1.0, healthMultiplier = 1.0},    -- Minions
                    [2] = {damageMultiplier = 1.4, healthMultiplier = 1.3},    -- Lieutenants
                    [3] = {damageMultiplier = 2.2, healthMultiplier = 1.8}     -- Boss Pet (MUCH higher)
                }
            }
        },
        
        individualScaling = {
            BOSS_PET = {
                levelScaling = 0.08,
                damageMultiplier = 2.5,
                healthMultiplier = 2.0,
                critChanceBonus = 15,
                tier = 3
            }
        }
    }
}
