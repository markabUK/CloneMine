-- Mastermind Pet Definitions
return {
    pets = {
        MINION = {
            name = "Henchman",
            tier = 1,
            type = "MELEE_DPS",
            baseHealth = 60,
            healthPerLevel = 6,
            baseDamage = 10,
            damagePerLevel = 1.2,
            baseArmor = 4,
            armorPerLevel = 0.6,
            attackSpeed = 1.3,
            attackRange = 5,
            moveSpeed = 100,
            behavior = "AGGRESSIVE",
            abilities = {
                {name = "Punch", damage = 12, cooldown = 3},
                {name = "Flurry", damage = 8, hits = 3, cooldown = 15}
            },
            specialization = "DPS",
            -- Pet-specific damage scaling override
            damageScalingOverride = {
                baseDamageMultiplier = 0.5,  -- 50% of player damage
                ownerStatScaling = {
                    CHA = 0.03  -- +3% damage per owner's CHA
                },
                levelScaling = 0.05  -- +5% per pet level
            }
        },
        LIEUTENANT = {
            name = "Elite Enforcer",
            tier = 2,
            type = "RANGED_DPS",
            baseHealth = 80,
            healthPerLevel = 8,
            baseDamage = 14,
            damagePerLevel = 1.6,
            baseArmor = 5,
            armorPerLevel = 0.8,
            attackSpeed = 1.0,
            attackRange = 25,
            moveSpeed = 105,
            behavior = "AGGRESSIVE",
            abilities = {
                {name = "Snipe", damage = 20, cooldown = 8},
                {name = "Suppress", description = "-25% target damage for 8s", cooldown = 20}
            },
            specialization = "DPS",
            -- Pet-specific damage scaling override
            damageScalingOverride = {
                baseDamageMultiplier = 0.7,  -- 70% of player damage
                ownerStatScaling = {
                    CHA = 0.05  -- +5% damage per owner's CHA
                },
                levelScaling = 0.07  -- +7% per pet level
            }
        },
        BOSS_PET = {
            name = "Champion",
            tier = 3,
            type = "TANK_DPS",
            baseHealth = 120,
            healthPerLevel = 12,
            baseDamage = 18,
            damagePerLevel = 2,
            baseArmor = 8,
            armorPerLevel = 1.2,
            attackSpeed = 1.4,
            attackRange = 5,
            moveSpeed = 100,
            behavior = "DEFENSIVE",
            abilities = {
                {name = "Devastate", damage = 30, cooldown = 10},
                {name = "Intimidate", description = "AOE fear 6s", cooldown = 45},
                {name = "Rally", description = "Buff nearby pets +20% damage", cooldown = 60}
            },
            specialization = "TANK/DPS",
            -- Boss Pet gets SUPERIOR scaling (MUCH stronger than other pets)
            damageScalingOverride = {
                baseDamageMultiplier = 1.2,  -- 120% of player damage (higher than player!)
                ownerStatScaling = {
                    CHA = 0.08,  -- +8% damage per owner's CHA (HUGE)
                    STR = 0.02   -- Also benefits from owner's STR
                },
                levelScaling = 0.10,  -- +10% per pet level
                critChanceBonus = 0.15,  -- +15% crit chance
                critDamageMultiplier = 1.5  -- Boss pet crits hit 50% harder
            }
        }
    },
    petScaling = {healthScaling = 0.7, damageScaling = 0.6, armorScaling = 0.5},
    commands = {"ATTACK", "FOLLOW", "STAY", "PASSIVE", "DEFENSIVE", "AGGRESSIVE"}
}
