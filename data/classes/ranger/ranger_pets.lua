-- Ranger Pet Definitions
-- Animal companion stat blocks

return {
    pets = {
        WOLF = {
            name = "Wolf Companion",
            type = "MELEE_DPS",
            
            baseHealth = 80,
            healthPerLevel = 8,
            baseDamage = 12,
            damagePerLevel = 1.5,
            baseArmor = 5,
            armorPerLevel = 0.8,
            
            attackSpeed = 1.2,
            attackRange = 5,
            moveSpeed = 110,
            
            behavior = "AGGRESSIVE",
            
            abilities = {
                {name = "Bite", damage = 15, cooldown = 5},
                {name = "Furious Howl", description = "+10% party damage for 10s", cooldown = 40}
            },
            
            specialization = "DPS"
        },
        
        BEAR = {
            name = "Bear Companion",
            type = "TANK",
            
            baseHealth = 120,
            healthPerLevel = 12,
            baseDamage = 8,
            damagePerLevel = 1,
            baseArmor = 8,
            armorPerLevel = 1.2,
            
            attackSpeed = 1.5,
            attackRange = 5,
            moveSpeed = 95,
            
            behavior = "DEFENSIVE",
            
            abilities = {
                {name = "Claw", damage = 12, cooldown = 6},
                {name = "Growl", description = "Taunt enemy", cooldown = 10},
                {name = "Thick Hide", description = "+10% armor passive"}
            },
            
            specialization = "TANK"
        },
        
        HAWK = {
            name = "Hawk Companion",
            type = "RANGED_DPS",
            
            baseHealth = 60,
            healthPerLevel = 6,
            baseDamage = 14,
            damagePerLevel = 1.8,
            baseArmor = 3,
            armorPerLevel = 0.5,
            
            attackSpeed = 1.0,
            attackRange = 30,
            moveSpeed = 120,
            
            behavior = "AGGRESSIVE",
            canFly = true,
            
            abilities = {
                {name = "Dive", damage = 20, cooldown = 8},
                {name = "Screech", description = "Reduce attack speed 25% in 10yd", cooldown = 30}
            },
            
            specialization = "DPS"
        }
    },
    
    -- Pet leveling
    petScaling = {
        healthScaling = 0.7,  -- Pet has 70% of player's health scaling
        damageScaling = 0.6,  -- Pet deals 60% of player's damage
        armorScaling = 0.5    -- Pet has 50% of player's armor
    },
    
    -- Pet commands
    commands = {
        "ATTACK",
        "FOLLOW",
        "STAY",
        "PASSIVE",
        "DEFENSIVE",
        "AGGRESSIVE"
    }
}
