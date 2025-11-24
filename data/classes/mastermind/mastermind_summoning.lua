-- Mastermind Summoning System (Updated)
-- Pet controller with customizable summon counts

return {
    -- Maximum total summons
    maxSummons = 6,
    
    -- Summon tiers and customizable limits
    summonTiers = {
        {
            tier = 1,
            name = "Minions",
            maxCount = 3,  -- Can summon up to 3 basic minions
            unlockLevel = 1
        },
        {
            tier = 2,
            name = "Lieutenants",
            maxCount = 2,  -- Can summon up to 2 lieutenants
            unlockLevel = 10
        },
        {
            tier = 3,
            name = "Boss Pet",
            maxCount = 1,  -- Can summon 1 boss pet
            unlockLevel = 20
        }
    },
    
    -- Summon definitions
    summons = {
        -- Tier 1: Basic Minions (3 max)
        MINION = {
            name = "Henchman",
            tier = 1,
            level = 1,
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
            followDistance = 10,
            aggroRange = 20,
            
            abilities = {
                {
                    name = "Power Strike",
                    damage = 15,
                    cooldown = 8,
                    description = "A powerful melee strike"
                }
            },
            
            model = "HENCHMAN",
            size = 1.0,
            
            duration = 0,  -- Permanent
            despawnOnMasterDeath = true
        },
        
        -- Tier 2: Lieutenants (2 max)
        LIEUTENANT = {
            name = "Elite Enforcer",
            tier = 2,
            level = 10,
            type = "MELEE_DPS",
            
            baseHealth = 100,
            healthPerLevel = 10,
            baseDamage = 18,
            damagePerLevel = 2,
            baseArmor = 7,
            armorPerLevel = 1,
            
            attackSpeed = 1.5,
            attackRange = 5,
            moveSpeed = 110,
            
            behavior = "AGGRESSIVE",
            followDistance = 12,
            aggroRange = 25,
            
            abilities = {
                {
                    name = "Crushing Blow",
                    damage = 35,
                    cooldown = 12,
                    stun = 2,
                    description = "Stuns the target"
                },
                {
                    name = "Rally Cry",
                    cooldown = 30,
                    duration = 15,
                    buffAllAllies = true,
                    damageBonus = 20,
                    description = "Buffs all nearby allies"
                }
            },
            
            model = "ENFORCER",
            size = 1.15,
            
            duration = 0,  -- Permanent
            despawnOnMasterDeath = true
        },
        
        -- Tier 3: Boss Pet (1 max)
        BOSS_PET = {
            name = "Mastermind's Champion",
            tier = 3,
            level = 20,
            type = "ELITE",
            
            baseHealth = 180,
            healthPerLevel = 18,
            baseDamage = 28,
            damagePerLevel = 3,
            baseArmor = 12,
            armorPerLevel = 1.5,
            
            attackSpeed = 1.4,
            attackRange = 5,
            moveSpeed = 115,
            critChance = 20,
            
            behavior = "SMART_AGGRESSIVE",
            followDistance = 15,
            aggroRange = 30,
            protectMaster = true,
            
            abilities = {
                {
                    name = "Devastating Strike",
                    damage = 60,
                    cooldown = 10,
                    armorPenetration = 50,
                    description = "Ignores half of target's armor"
                },
                {
                    name = "Protective Stance",
                    cooldown = 45,
                    duration = 12,
                    taunt = true,
                    damageReduction = 40,
                    threatMultiplier = 3,
                    description = "Taunts enemies and reduces damage taken"
                },
                {
                    name = "Champion's Roar",
                    cooldown = 60,
                    duration = 20,
                    buffAllAllies = true,
                    damageBonus = 30,
                    attackSpeedBonus = 25,
                    armorBonus = 10,
                    description = "Powerful buff for all allies"
                }
            },
            
            -- Special features
            lifeSteal = 15,
            cannotBeFeared = true,
            cannotBeStunned = true,
            
            model = "CHAMPION",
            size = 1.3,
            
            duration = 0,  -- Permanent
            despawnOnMasterDeath = true
        }
    },
    
    -- Summoning mechanics
    mechanics = {
        -- No corpse requirements for Mastermind
        requiresCorpse = false,
        
        -- Control mechanics
        commandRange = 60,
        commandCooldown = 0.5,
        preciseControl = true,  -- Mastermind has superior pet control
        
        -- Pet scaling (scales with CHA)
        petScaling = {
            healthFromMasterCHA = 3,  -- +3% health per point of master CHA
            damageFromMasterCHA = 2,  -- +2% damage per point of master CHA
            armorFromMasterCHA = 1    -- +1% armor per point of master CHA
        },
        
        -- Upgrade system
        upgrades = {
            {
                level = 5,
                description = "Minions gain +20% health",
                effect = {target = "MINION", healthBonus = 20}
            },
            {
                level = 8,
                description = "All pets gain +15% damage",
                effect = {target = "ALL", damageBonus = 15}
            },
            {
                level = 15,
                description = "Lieutenants gain an additional ability",
                effect = {target = "LIEUTENANT", abilityUnlock = "Intimidate"}
            },
            {
                level = 18,
                description = "Boss Pet becomes immune to crowd control",
                effect = {target = "BOSS_PET", ccImmune = true}
            }
        },
        
        // Death handling
        onPetDeath = {
            manaRefund = 0,  // No mana cost to summon
            canReSummon = true,
            reSummonCooldown = 30  -- 30 second cooldown to re-summon
        },
        
        // Master death
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
        petCommandWheel = true,
        formationControl = true  // Mastermind can set pet formations
    }
}
