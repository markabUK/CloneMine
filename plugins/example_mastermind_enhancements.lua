-- Mastermind Enhancement Spells
-- Special spells for upgrading and enhancing pets

return {
    -- Add these spells to data/spells/player_spells.lua
    mastermindSpells = {
        {
            name = "Upgrade Pet",
            school = "Arcane",
            manaCost = 80,
            castTime = 3.0,
            cooldown = 120,
            range = 40,
            classes = {"MASTERMIND"},
            targetType = "PET",
            
            -- Visual effects (references spell_tints.lua)
            tint = "Upgrade Pet",  -- Custom tint defined in spell_tints.lua
            
            effect = "UPGRADE_PET_TIER",
            description = "Upgrades your pet to the next tier. Minion -> Lieutenant, Lieutenant -> Boss",
            
            upgradePath = {
                MINION = "LIEUTENANT",
                LIEUTENANT = "BOSS_PET"
            },
            
            -- Upgrade bonuses
            upgradeBonus = {
                healthIncrease = 40,  -- +40% health
                damageIncrease = 50,  -- +50% damage
                armorIncrease = 30,   -- +30% armor
                newAbility = true     -- Unlocks tier-specific ability
            },
            
            -- Requirements
            requirements = {
                MINION = {level = 10, gold = 500},
                LIEUTENANT = {level = 20, gold = 2000, item = "Superior Command Insignia"}
            },
            
            -- Can only upgrade one pet at a time
            maxUpgrades = {
                tier2 = 2,  -- Max 2 Lieutenants
                tier3 = 1   -- Max 1 Boss Pet
            }
        },
        
        {
            name = "Enhance Pets",
            school = "Arcane",
            manaCost = 60,
            castTime = 2.0,
            cooldown = 60,
            range = 0,  -- Self-cast AOE
            aoe = {radius = 40, targetType = "PETS_ONLY"},
            duration = 30,
            classes = {"MASTERMIND"},
            
            -- Base damage for scaling (this buff adds damage to pets)
            baseDamage = 0,  -- No direct damage, but adds to pet damage
            
            -- Visual effects
            tint = "Enhance Pets",
            
            effect = "ENHANCE_ALL_PETS",
            description = "Enhances all your pets for 30 seconds with powerful buffs",
            
            -- Different enhancements based on pet tier
            enhancements = {
                -- Tier 1: Minions
                [1] = {
                    name = "Minion Enhancement",
                    effects = {
                        {type = "DAMAGE_INCREASE", value = 30},      -- +30% damage
                        {type = "ATTACK_SPEED", value = 20},         -- +20% attack speed
                        {type = "MOVEMENT_SPEED", value = 15},       -- +15% move speed
                        {type = "ABILITY_COOLDOWN", value = -20}     -- -20% ability cooldowns
                    },
                    newAbility = {
                        name = "Coordinated Strike",
                        baseDamage = 80,  -- Base damage (scales with Mastermind stats)
                        description = "All minions strike simultaneously",
                        cooldown = 15
                    }
                },
                
                -- Tier 2: Lieutenants
                [2] = {
                    name = "Lieutenant Enhancement",
                    effects = {
                        {type = "DAMAGE_INCREASE", value = 40},      -- +40% damage
                        {type = "ARMOR_INCREASE", value = 50},       -- +50% armor
                        {type = "HEALTH_REGEN", value = 5},          -- +5% HP regen/sec
                        {type = "CRITICAL_CHANCE", value = 15},      -- +15% crit chance
                        {type = "LIFESTEAL", value = 10}             -- 10% lifesteal
                    },
                    newAbility = {
                        name = "Commander's Aura",
                        baseDamage = 0,  -- Support ability
                        description = "Grants nearby allies +10% damage",
                        aoeRadius = 15,
                        cooldown = 0  -- Passive aura
                    }
                },
                
                -- Tier 3: Boss Pet
                [3] = {
                    name = "Champion Enhancement",
                    effects = {
                        {type = "DAMAGE_INCREASE", value = 60},      -- +60% damage
                        {type = "HEALTH_INCREASE", value = 50},      -- +50% health
                        {type = "ARMOR_INCREASE", value = 75},       -- +75% armor
                        {type = "CRITICAL_DAMAGE", value = 50},      -- +50% crit damage
                        {type = "ABILITY_POWER", value = 40},        -- +40% ability damage
                        {type = "LIFESTEAL", value = 20},            -- 20% lifesteal
                        {type = "CLEAVE", value = 50}                -- Attacks hit 50% AOE
                    },
                    newAbility = {
                        name = "Domination",
                        baseDamage = 300,  -- Very high base damage (scales heavily)
                        description = "Devastating attack that fears nearby enemies",
                        cooldown = 30,
                        aoeRadius = 10,
                        crowdControl = "Fear",
                        fearDuration = 4
                    }
                },
                
                -- All tiers also get these universal buffs
                universal = {
                    effects = {
                        {type = "DAMAGE_RESISTANCE", value = 20},    -- +20% damage reduction
                        {type = "HEALING_RECEIVED", value = 30},     -- +30% healing received
                        {type = "STATUS_RESIST", value = 50}         -- +50% CC resistance
                    }
                }
            },
            
            -- Stat bonuses (scale with Mastermind's CHA)
            statScaling = {
                damagePerCHA = 0.5,    -- Each point of CHA adds 0.5% to pet damage bonus
                armorPerCHA = 0.3,     -- Each point of CHA adds 0.3% to pet armor bonus
                durationPerCHA = 0.1   -- Each point of CHA adds 0.1 seconds to duration
            }
        },
        
        {
            name = "Supreme Command",
            school = "Arcane",
            manaCost = 100,
            castTime = 0,
            cooldown = 180,
            range = 0,
            duration = 20,
            classes = {"MASTERMIND"},
            
            -- Base damage - pets deal massive damage during this
            baseDamage = 0,
            
            effect = "ULTIMATE_PET_BUFF",
            description = "Ultimate ability: All pets become unstoppable juggernauts for 20 seconds",
            
            enhancements = {
                {type = "DAMAGE_INCREASE", value = 100},     -- Double damage
                {type = "HEALTH_INCREASE", value = 75},      -- +75% health
                {type = "ATTACK_SPEED", value = 50},         -- +50% attack speed
                {type = "MOVEMENT_SPEED", value = 40},       -- +40% move speed
                {type = "IMMUNITY_CC", value = true},        -- Immune to crowd control
                {type = "DAMAGE_REDUCTION", value = 50},     -- +50% damage reduction
                {type = "LIFESTEAL", value = 30},            -- 30% lifesteal
                {type = "ABILITY_COOLDOWN", value = -50}     -- -50% cooldowns
            },
            
            visualEffect = {
                petAura = {r = 255, g = 215, b = 0},  -- Golden aura
                particles = "INTENSE",
                size = 1.3  -- Pets appear 30% larger
            },
            
            unlockLevel = 20  -- Ultimate ability at max level
        },
        
        {
            name = "Call Reinforcements",
            school = "Physical",
            manaCost = 120,
            castTime = 3.0,
            cooldown = 300,
            range = 10,
            classes = {"MASTERMIND"},
            
            baseDamage = 0,  -- Summon spell
            
            effect = "SUMMON_TEMPORARY_PETS",
            description = "Summons 3 temporary Minions for 60 seconds",
            
            summonDetails = {
                petType = "MINION",
                count = 3,
                duration = 60,
                powerLevel = 0.8  -- 80% of normal minion power (temporary)
            },
            
            -- These don't count against pet limit but are weaker
            temporary = true,
            
            unlockLevel = 15
        },
        
        {
            name = "Pet Sacrifice",
            school = "Shadow",
            manaCost = 40,
            castTime = 1.0,
            cooldown = 45,
            range = 40,
            targetType = "PET",
            classes = {"MASTERMIND"},
            
            baseDamage = 200,  -- Explosion base damage (scales with pet's health)
            
            effect = "SACRIFICE_PET",
            description = "Sacrifice a pet to deal massive damage based on its remaining health",
            
            damageCalculation = {
                basePercentage = 200,  -- 200% of pet's current health as damage
                aoeRadius = 15,
                damageType = "Shadow"
            },
            
            masterBonus = {
                healing = 30,  -- Master heals for 30% of damage dealt
                manaRegen = 50  -- Restores 50 mana
            }
        },
        
        {
            name = "Tactical Repositioning",
            school = "Arcane",
            manaCost = 30,
            castTime = 0,
            cooldown = 20,
            range = 40,
            classes = {"MASTERMIND"},
            
            baseDamage = 0,  -- Utility spell
            
            effect = "TELEPORT_PETS",
            description = "Instantly teleport all pets to target location",
            
            utilityEffect = {
                teleportType = "GROUND_TARGET",
                radius = 10,  -- Pets spread in 10 yard radius
                removesCrowdControl = true  -- Also breaks CC on pets
            }
        }
    },
    
    -- Integration notes
    integrationGuide = {
        note = "Add these spells to Mastermind class in data/classes/mastermind/mastermind_spells.lua",
        
        spellUnlocks = {
            [1] = {"Enhance Pets"},
            [10] = {"Upgrade Pet", "Call Reinforcements"},
            [15] = {"Tactical Repositioning"},
            [18] = {"Pet Sacrifice"},
            [20] = {"Supreme Command"}
        }
    }
}
