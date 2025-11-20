-- Example: Adding New Pets/Summons via Plugin
-- This demonstrates adding new pets for existing classes

return {
    -- Add new pets to existing classes
    registerPets = function(gameAPI)
        
        -- ===========================
        -- Add new Ranger pet: Eagle
        -- ===========================
        gameAPI.addPet({
            class = "RANGER",
            petId = "EAGLE",
            name = "Eagle Companion",
            type = "RANGED_DPS",
            
            baseHealth = 60,
            healthPerLevel = 6,
            baseDamage = 16,
            damagePerLevel = 2,
            baseArmor = 3,
            armorPerLevel = 0.5,
            
            attackSpeed = 0.8,
            attackRange = 35,
            moveSpeed = 120,
            canFly = true,
            
            behavior = "AGGRESSIVE",
            
            -- Reference spells from monster_spells.lua
            abilities = {
                {name = "Dive Bomb"},  -- New spell defined below
                {name = "Screech"},    -- New spell
                {name = "Keen Eye"}    -- New spell
            },
            
            specialization = "RANGED_DPS"
        })
        
        -- Define new pet spells in monster_spells.lua
        gameAPI.addMonsterSpell({
            name = "Dive Bomb",
            school = "Physical",
            castTime = 1.0,
            cooldown = 12,
            range = 40,
            damage = 85,
            damageType = "Physical",
            description = "Eagle dives at target dealing heavy damage"
        })
        
        gameAPI.addMonsterSpell({
            name = "Screech",
            school = "Physical",
            castTime = 0,
            cooldown = 30,
            aoe = {radius = 15},
            crowdControl = "Fear",
            duration = 3,
            description = "Screeches, fearing nearby enemies for 3 seconds"
        })
        
        gameAPI.addMonsterSpell({
            name = "Keen Eye",
            school = "Physical",
            castTime = 0,
            cooldown = 60,
            duration = 15,
            buff = {stat = "DEX", amount = 10, target = "master"},
            description = "Grants master +10 DEX for 15 seconds"
        })
        
        -- ===========================
        -- Add new Warlock pet: Felhunter
        -- ===========================
        gameAPI.addPet({
            class = "WARLOCK",
            petId = "FELHUNTER",
            name = "Felhunter",
            type = "UTILITY",
            
            baseHealth = 80,
            healthPerLevel = 8,
            baseDamage = 10,
            damagePerLevel = 1.3,
            baseArmor = 5,
            armorPerLevel = 0.8,
            
            attackSpeed = 1.5,
            attackRange = 5,
            moveSpeed = 105,
            
            behavior = "DEFENSIVE",
            
            abilities = {
                {name = "Spell Lock"},     -- New spell
                {name = "Devour Magic"},   -- New spell
                {name = "Shadow Bite"}     -- New spell
            },
            
            specialization = "UTILITY"
        })
        
        gameAPI.addMonsterSpell({
            name = "Spell Lock",
            school = "Shadow",
            castTime = 0,
            cooldown = 24,
            range = 30,
            crowdControl = "Silence",
            duration = 4,
            description = "Silences target for 4 seconds"
        })
        
        gameAPI.addMonsterSpell({
            name = "Devour Magic",
            school = "Arcane",
            castTime = 0,
            cooldown = 15,
            range = 30,
            removesBuffs = 1,
            healing = 50,
            description = "Removes 1 magic effect and heals Felhunter"
        })
        
        gameAPI.addMonsterSpell({
            name = "Shadow Bite",
            school = "Shadow",
            castTime = 0,
            cooldown = 6,
            range = 5,
            damage = 60,
            damageType = "Shadow",
            bonusDamagePerBuff = 20,
            description = "Deals extra damage per buff on target"
        })
        
        -- ===========================
        -- Add new Necromancer pet: Lich
        -- ===========================
        gameAPI.addPet({
            class = "NECROMANCER",
            petId = "LICH",
            name = "Risen Lich",
            type = "CASTER",
            tier = 4, -- Special high-tier pet
            
            baseHealth = 90,
            healthPerLevel = 9,
            baseDamage = 20,
            damagePerLevel = 2.5,
            baseArmor = 2,
            armorPerLevel = 0.3,
            
            attackSpeed = 2.5,
            attackRange = 35,
            moveSpeed = 90,
            
            behavior = "AGGRESSIVE",
            isUndead = true,
            
            abilities = {
                {name = "Death Coil"},      -- New spell
                {name = "Frost Nova"},      -- Existing from monster_spells
                {name = "Animate Dead"},    -- New spell
                {name = "Phylactery"}       -- New spell (passive)
            },
            
            specialization = "CASTER",
            
            -- Special requirement
            summonRequirement = {
                level = 18,
                reagent = "Phylactery of Undeath",
                cooldown = 600
            }
        })
        
        gameAPI.addMonsterSpell({
            name = "Death Coil",
            school = "Shadow",
            castTime = 0,
            cooldown = 8,
            range = 30,
            damage = 100,
            damageType = "Shadow",
            healing = 100, -- Heals undead
            smartCast = true,
            description = "Damages living or heals undead"
        })
        
        gameAPI.addMonsterSpell({
            name = "Animate Dead",
            school = "Necromancy",
            castTime = 3.0,
            cooldown = 60,
            range = 30,
            summons = "Skeleton Minion",
            summonCount = 2,
            summonDuration = 30,
            description = "Raises 2 skeleton minions for 30 seconds"
        })
        
        gameAPI.addMonsterSpell({
            name = "Phylactery",
            school = "Necromancy",
            passive = true,
            effect = "Lich respawns 30 seconds after death",
            description = "Lich cannot be permanently killed"
        })
        
        -- ===========================
        -- Add new Mastermind pet: Bruiser (new tier)
        -- ===========================
        gameAPI.addPet({
            class = "MASTERMIND",
            petId = "BRUISER",
            name = "Bruiser",
            tier = 2, -- Lieutenant-level
            type = "MELEE_DPS",
            
            baseHealth = 90,
            healthPerLevel = 9,
            baseDamage = 20,
            damagePerLevel = 2.5,
            baseArmor = 6,
            armorPerLevel = 1,
            
            attackSpeed = 1.3,
            attackRange = 5,
            moveSpeed = 105,
            
            behavior = "AGGRESSIVE",
            
            abilities = {
                {name = "Haymaker"},        -- New spell
                {name = "Intimidate"},      -- New spell
                {name = "Headbutt"}         -- New spell
            },
            
            specialization = "DPS"
        })
        
        gameAPI.addMonsterSpell({
            name = "Haymaker",
            school = "Physical",
            castTime = 0,
            cooldown = 10,
            range = 5,
            damage = 90,
            damageType = "Physical",
            crowdControl = "Knockback",
            distance = 10,
            description = "Powerful punch that knocks back target"
        })
        
        gameAPI.addMonsterSpell({
            name = "Intimidate",
            school = "Physical",
            castTime = 0,
            cooldown = 30,
            aoe = {radius = 10},
            debuff = {stat = "damage", amount = -20, duration = 10},
            description = "Reduces nearby enemy damage by 20%"
        })
        
        gameAPI.addMonsterSpell({
            name = "Headbutt",
            school = "Physical",
            castTime = 0,
            cooldown = 15,
            range = 5,
            damage = 60,
            damageType = "Physical",
            crowdControl = "Stun",
            duration = 2,
            description = "Stuns target for 2 seconds"
        })
        
        -- ===========================
        -- Update summoning limits (optional)
        -- ===========================
        gameAPI.updatePetLimits({
            class = "RANGER",
            maxPets = 1,
            availablePets = {"WOLF", "BEAR", "HAWK", "EAGLE"} -- Added EAGLE
        })
        
        gameAPI.updatePetLimits({
            class = "WARLOCK",
            maxPets = 1,
            availablePets = {"IMP", "VOIDWALKER", "SUCCUBUS", "FELHUNTER"} -- Added FELHUNTER
        })
        
        gameAPI.updatePetLimits({
            class = "NECROMANCER",
            maxPets = 6, -- Increased from 5
            tiers = {
                {tier = 1, max = 3, pets = {"SKELETON_MINION"}},
                {tier = 2, max = 1, pets = {"ZOMBIE_TANK"}},
                {tier = 3, max = 1, pets = {"GHOUL_ELITE"}},
                {tier = 4, max = 1, pets = {"LICH"}} -- New tier
            }
        })
    end
}
