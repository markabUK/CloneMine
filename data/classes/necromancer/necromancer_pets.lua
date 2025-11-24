-- Necromancer Pet Definitions
return {
    pets = {
        SKELETON_MINION = {
            name = "Skeleton Warrior",
            tier = 1,
            type = "MELEE_DPS",
            baseHealth = 50,
            healthPerLevel = 5,
            baseDamage = 8,
            damagePerLevel = 1,
            baseArmor = 3,
            armorPerLevel = 0.5,
            attackSpeed = 1.5,
            attackRange = 5,
            moveSpeed = 100,
            behavior = "AGGRESSIVE",
            isUndead = true,
            abilities = {
                {name = "Bone Strike", damage = 10, cooldown = 4},
                {name = "Rattle", description = "Reduce enemy accuracy 10%", cooldown = 20}
            },
            specialization = "DPS"
        },
        ZOMBIE_TANK = {
            name = "Zombie Brute",
            tier = 2,
            type = "TANK",
            baseHealth = 100,
            healthPerLevel = 10,
            baseDamage = 6,
            damagePerLevel = 0.8,
            baseArmor = 6,
            armorPerLevel = 1,
            attackSpeed = 2.0,
            attackRange = 5,
            moveSpeed = 80,
            behavior = "DEFENSIVE",
            isUndead = true,
            abilities = {
                {name = "Slam", damage = 12, stun = 2, cooldown = 10},
                {name = "Plague Aura", description = "Nearby enemies take damage over time", cooldown = 0}
            },
            specialization = "TANK"
        },
        GHOUL_ELITE = {
            name = "Ghoul Ravager",
            tier = 3,
            type = "MELEE_DPS",
            baseHealth = 70,
            healthPerLevel = 7,
            baseDamage = 16,
            damagePerLevel = 2,
            baseArmor = 4,
            armorPerLevel = 0.7,
            attackSpeed = 1.2,
            attackRange = 5,
            moveSpeed = 110,
            behavior = "AGGRESSIVE",
            isUndead = true,
            abilities = {
                {name = "Rend", damage = 20, bleed = 40, cooldown = 8},
                {name = "Frenzy", description = "+50% attack speed for 10s", cooldown = 30},
                {name = "Feast", description = "Consume corpse to heal", cooldown = 15}
            },
            specialization = "DPS"
        }
    },
    petScaling = {healthScaling = 0.6, damageScaling = 0.5, armorScaling = 0.4},
    commands = {"ATTACK", "FOLLOW", "STAY", "PASSIVE", "DEFENSIVE", "AGGRESSIVE"},
    undeadBonuses = {
        immuneToPo ison = true,
        immuneToFear = true,
        immuneToDiseaseuntil20% = true,
        healedByDeathMagic = true
    }
}
