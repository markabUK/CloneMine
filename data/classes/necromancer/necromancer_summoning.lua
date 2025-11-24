-- Necromancer Summoning System
-- Undead minion definitions and summoning mechanics

return {
    -- Maximum total summons
    maxSummons = 5,
    
    -- Summon tiers and limits
    summonTiers = {
        {
            tier = 1,
            name = "Skeleton Minions",
            maxCount = 3,
            unlockLevel = 1
        },
        {
            tier = 2,
            name = "Zombie Tank",
            maxCount = 1,
            unlockLevel = 6
        },
        {
            tier = 3,
            name = "Ghoul Elite",
            maxCount = 1,
            unlockLevel = 15
        }
    },
    
    -- Summon definitions
    summons = {
        -- Tier 1: Skeleton Minion
        SKELETON_MINION = {
            name = "Skeleton Warrior",
            tier = 1,
            level = 1,
            type = "MELEE_DPS",
            
            -- Base stats (scale with necromancer level)
            baseHealth = 50,
            healthPerLevel = 5,
            baseDamage = 8,
            damagePerLevel = 1,
            baseArmor = 3,
            armorPerLevel = 0.5,
            
            -- Combat stats
            attackSpeed = 1.5,  -- Attacks per second
            attackRange = 5,    -- Melee range
            moveSpeed = 100,    -- Percentage of player speed
            
            -- AI behavior
            behavior = "AGGRESSIVE",
            followDistance = 10,
            aggroRange = 20,
            
            -- Abilities
            abilities = {
                {
                    name = "Bone Strike",
                    damage = 12,
                    cooldown = 5,
                    description = "Strikes with bone weapons"
                }
            },
            
            -- Appearance
            model = "SKELETON",
            size = 1.0,
            
            -- Lifespan
            duration = 0,  -- Permanent until killed or dismissed
            despawnOnMasterDeath = true
        },
        
        -- Tier 2: Zombie Tank
        ZOMBIE_TANK = {
            name = "Risen Zombie",
            tier = 2,
            level = 6,
            type = "TANK",
            
            -- Base stats (scale with necromancer level)
            baseHealth = 120,
            healthPerLevel = 12,
            baseDamage = 10,
            damagePerLevel = 1.5,
            baseArmor = 8,
            armorPerLevel = 1,
            
            -- Combat stats
            attackSpeed = 0.8,  -- Slower but harder hits
            attackRange = 5,
            moveSpeed = 70,     -- Slower movement
            
            -- Tank stats
            threat Multiplier = 2.0,  -- Generates extra threat
            taunt = true,
            
            -- AI behavior
            behavior = "DEFENSIVE",
            followDistance = 5,
            aggroRange = 15,
            protectMaster = true,
            
            -- Abilities
            abilities = {
                {
                    name = "Putrid Strike",
                    damage = 20,
                    cooldown = 8,
                    dot = {duration = 6, tickDamage = 3},
                    description = "Strikes with disease-ridden fists"
                },
                {
                    name = "Corpse Wall",
                    cooldown = 30,
                    absorbDamage = 50,
                    duration = 10,
                    description = "Creates a wall of flesh that absorbs damage"
                }
            },
            
            -- Appearance
            model = "ZOMBIE",
            size = 1.2,
            
            -- Lifespan
            duration = 0,  -- Permanent
            despawnOnMasterDeath = true
        },
        
        -- Tier 3: Ghoul Elite
        GHOUL_ELITE = {
            name = "Ravenous Ghoul",
            tier = 3,
            level = 15,
            type = "MELEE_DPS",
            
            -- Base stats (scale with necromancer level)
            baseHealth = 100,
            healthPerLevel = 10,
            baseDamage = 18,
            damagePerLevel = 2.5,
            baseArmor = 5,
            armorPerLevel = 0.8,
            
            -- Combat stats
            attackSpeed = 2.0,  -- Very fast attacks
            attackRange = 5,
            moveSpeed = 130,    -- Faster than player
            critChance = 15,    -- Can crit
            
            -- AI behavior
            behavior = "AGGRESSIVE",
            followDistance = 15,
            aggroRange = 25,
            preferLowHealthTargets = true,
            
            -- Abilities
            abilities = {
                {
                    name = "Frenzy",
                    cooldown = 20,
                    duration = 10,
                    attackSpeedBonus = 50,
                    damageBonus = 30,
                    description = "Enters a frenzied state"
                },
                {
                    name = "Rend Flesh",
                    damage = 30,
                    cooldown = 10,
                    bleed = {duration = 9, tickDamage = 5},
                    description = "Tears at the enemy, causing bleeding"
                },
                {
                    name = "Leap",
                    cooldown = 15,
                    range = 20,
                    damage = 25,
                    stun = 2,
                    description = "Leaps to a target, stunning them"
                }
            },
            
            -- Special features
            lifeSteal = 10,  -- Heals 10% of damage dealt
            
            -- Appearance
            model = "GHOUL",
            size = 1.1,
            
            -- Lifespan
            duration = 0,  // Permanent
            despawnOnMasterDeath = true
        },
        
        -- Special Summon: Temporary Army (from Army of the Dead)
        TEMPORARY_SKELETON = {
            name = "Risen Skeleton",
            tier = 0,  // Special tier
            level = 1,
            type = "MELEE_DPS",
            
            baseHealth = 40,
            healthPerLevel = 3,
            baseDamage = 10,
            damagePerLevel = 1.2,
            baseArmor = 2,
            armorPerLevel = 0.3,
            
            attackSpeed = 1.3,
            attackRange = 5,
            moveSpeed = 110,
            
            behavior = "AGGRESSIVE",
            followDistance = 20,
            aggroRange = 30,
            
            model = "SKELETON_WARRIOR",
            size = 0.9,
            
            duration = 40,  // Temporary - 40 seconds
            despawnOnMasterDeath = true,
            doesNotCountTowardLimit = true
        },
        
        -- Special Summon: Death Knight (from Apocalypse)
        DEATH_KNIGHT = {
            name = "Death Knight",
            tier = 0,  // Special tier
            level = 20,
            type = "ELITE",
            
            baseHealth = 200,
            healthPerLevel = 15,
            baseDamage = 30,
            damagePerLevel = 3,
            baseArmor = 15,
            armorPerLevel = 1.5,
            
            attackSpeed = 1.2,
            attackRange = 5,
            moveSpeed = 100,
            
            behavior = "AGGRESSIVE",
            followDistance = 15,
            aggroRange = 30,
            
            abilities = {
                {
                    name = "Death Strike",
                    damage = 50,
                    cooldown = 8,
                    lifesteal = 100,
                    description = "Powerful strike that heals the Death Knight"
                },
                {
                    name = "Icebound Fortitude",
                    cooldown = 30,
                    duration = 8,
                    damageReduction = 50,
                    description = "Reduces damage taken"
                }
            },
            
            model = "DEATH_KNIGHT",
            size = 1.3,
            
            duration = 60,  // Temporary - 60 seconds
            despawnOnMasterDeath = true,
            doesNotCountTowardLimit = true
        }
    },
    
    -- Summoning mechanics
    mechanics = {
        -- Corpse requirements
        requiresCorpse = {
            SKELETON_MINION = true,
            ZOMBIE_TANK = true,
            GHOUL_ELITE = true
        },
        
        -- Control mechanics
        commandRange = 50,
        commandCooldown = 1,
        
        -- Pet scaling
        petScaling = {
            healthFromMasterINT = 2,  -- +2% health per point of master INT
            damageFromMasterINT = 1.5,  -- +1.5% damage per point of master INT
            armorFromMasterINT = 0.5   -- +0.5% armor per point of master INT
        },
        
        -- Death handling
        onPetDeath = {
            manaRefund = 20,  // Refund 20% of summon cost
            canBeRaisedAgain = false,  // Pet corpse cannot be raised
            leavesCorpse = true  // But can be used for Corpse Explosion
        },
        
        -- Master death
        onMasterDeath = {
            permanentPetsDie = true,
            temporaryPetsRemain = false
        }
    },
    
    -- UI information
    ui = {
        petFrameEnabled = true,
        showPetHealth = true,
        showPetMana = false,
        showPetBuffs = true,
        petCommandWheel = true
    }
}
