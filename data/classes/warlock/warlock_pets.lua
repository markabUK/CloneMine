-- Warlock Pet Definitions
return {
    pets = {
        IMP = {
            name = "Imp",
            type = "RANGED_DPS",
            baseHealth = 50,
            healthPerLevel = 5,
            baseDamage = 14,
            damagePerLevel = 1.8,
            baseArmor = 2,
            armorPerLevel = 0.4,
            attackSpeed = 1.0,
            attackRange = 30,
            moveSpeed = 100,
            behavior = "AGGRESSIVE",
            abilities = {
                {name = "Firebolt", damage = 18, cooldown = 2},
                {name = "Blood Pact", description = "+10% stamina to party"}
            },
            specialization = "DPS"
        },
        VOIDWALKER = {
            name = "Voidwalker",
            type = "TANK",
            baseHealth = 110,
            healthPerLevel = 11,
            baseDamage = 7,
            damagePerLevel = 0.9,
            baseArmor = 7,
            armorPerLevel = 1.1,
            attackSpeed = 1.8,
            attackRange = 5,
            moveSpeed = 90,
            behavior = "DEFENSIVE",
            abilities = {
                {name = "Torment", description = "Taunt enemy", cooldown = 8},
                {name = "Suffering", description = "AOE taunt", cooldown = 120},
                {name = "Sacrifice", description = "Shield master, die", cooldown = 300}
            },
            specialization = "TANK"
        },
        SUCCUBUS = {
            name = "Succubus",
            type = "MELEE_DPS",
            baseHealth = 70,
            healthPerLevel = 7,
            baseDamage = 16,
            damagePerLevel = 2,
            baseArmor = 4,
            armorPerLevel = 0.6,
            attackSpeed = 1.1,
            attackRange = 5,
            moveSpeed = 105,
            behavior = "AGGRESSIVE",
            abilities = {
                {name = "Lash of Pain", damage = 22, cooldown = 6},
                {name = "Seduction", description = "Charm enemy 18s", cooldown = 30}
            },
            specialization = "DPS"
        }
    },
    petScaling = {healthScaling = 0.6, damageScaling = 0.5, armorScaling = 0.4},
    commands = {"ATTACK", "FOLLOW", "STAY", "PASSIVE", "DEFENSIVE", "AGGRESSIVE"}
}
