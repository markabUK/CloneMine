-- Example: Adding a New Class via Plugin
-- This demonstrates how to create a completely new class (Paladin)

return {
    -- Register new class
    registerClass = function(gameAPI)
        gameAPI.addClass({
            name = "PALADIN",
            displayName = "Paladin",
            description = "Holy warrior combining martial prowess with divine magic",
            
            -- Stats (defined in class_stats format)
            stats = {
                primaryStat = "STR",
                secondaryStat = "WIS",
                baseHealth = 110,
                healthPerLevel = 11,
                resourceType = "MANA",
                baseResource = 100,
                resourcePerLevel = 8,
                
                baseStats = {
                    STR = 16,
                    DEX = 10,
                    CON = 14,
                    INT = 8,
                    WIS = 14,
                    CHA = 12
                },
                
                allowedArmor = {"PLATE", "MAIL", "LEATHER", "CLOTH"},
                allowedWeapons = {"SWORD", "MACE", "GREATSWORD", "SHIELD"},
                canDualWield = false,
                canUseShield = true
            },
            
            -- Spells (reference existing or new spells)
            spells = {
                "Holy Light",        -- From player_spells.lua
                "Divine Shield",     -- New spell (defined below)
                "Hammer of Justice", -- New spell
                "Consecration",      -- New spell
                "Holy Shock",        -- New spell
                "Blessing of Might", -- New spell
                "Crusader Strike",   -- New spell
                "Avenger's Shield"   -- New spell
            },
            
            spellUnlocks = {
                [1] = {"Holy Light", "Crusader Strike"},
                [3] = {"Hammer of Justice"},
                [5] = {"Blessing of Might"},
                [7] = {"Consecration"},
                [9] = {"Divine Shield"},
                [11] = {"Holy Shock"},
                [15] = {"Avenger's Shield"}
            },
            
            -- Abilities
            abilities = {
                "Lay on Hands",
                "Divine Protection",
                "Hand of Freedom",
                "Avenging Wrath",
                "Guardian of Kings"
            },
            
            abilityUnlocks = {
                [2] = {"Divine Protection"},
                [6] = {"Lay on Hands"},
                [10] = {"Hand of Freedom"},
                [14] = {"Avenging Wrath"},
                [18] = {"Guardian of Kings"}
            },
            
            -- Feats (earned every 2 levels)
            feats = {
                {name = "Holy Resolve", level = 2, effect = "+10% healing received"},
                {name = "Righteous Fury", level = 4, effect = "+15% threat generation"},
                {name = "Divine Purpose", level = 6, effect = "20% chance Holy attacks don't consume mana"},
                {name = "Sanctified Wrath", level = 8, effect = "+20% holy damage"},
                {name = "Unbreakable Spirit", level = 10, effect = "-30% cooldown on Divine Shield"},
                {name = "Holy Avenger", level = 12, effect = "+25% crit chance on holy spells"},
                {name = "Beacon of Light", level = 14, effect = "Healing also affects nearby ally"},
                {name = "Grand Crusader", level = 16, effect = "Melee attacks reduce spell cooldowns"},
                {name = "Light's Hammer", level = 18, effect = "Holy attacks deal AOE damage"},
                {name = "Divine Intervention", level = 20, effect = "Cheat death once per combat"}
            }
        })
        
        -- Register new spells for this class
        gameAPI.addPlayerSpell({
            name = "Divine Shield",
            school = "Holy",
            manaCost = 40,
            castTime = 0,
            cooldown = 300,
            duration = 8,
            classes = {"PALADIN"},
            description = "You become immune to all damage for 8 seconds"
        })
        
        gameAPI.addPlayerSpell({
            name = "Hammer of Justice",
            school = "Holy",
            manaCost = 25,
            castTime = 0,
            cooldown = 60,
            range = 10,
            damage = 30,
            damageType = "Holy",
            crowdControl = "Stun",
            duration = 6,
            classes = {"PALADIN"},
            description = "Stuns the target for 6 seconds"
        })
        
        gameAPI.addPlayerSpell({
            name = "Consecration",
            school = "Holy",
            manaCost = 50,
            castTime = 0,
            cooldown = 30,
            aoe = {radius = 8},
            duration = 10,
            dot = {tickDamage = 15, tickInterval = 1},
            classes = {"PALADIN"},
            description = "Consecrates the ground, dealing holy damage to enemies"
        })
        
        gameAPI.addPlayerSpell({
            name = "Holy Shock",
            school = "Holy",
            manaCost = 35,
            castTime = 0,
            cooldown = 6,
            range = 40,
            damage = 80,
            healing = 100,
            smartCast = true, -- Damage enemy or heal ally
            classes = {"PALADIN"},
            description = "Deals damage to enemy or heals ally"
        })
        
        gameAPI.addPlayerSpell({
            name = "Blessing of Might",
            school = "Holy",
            manaCost = 20,
            castTime = 0,
            cooldown = 0,
            range = 40,
            duration = 600,
            buff = {stat = "STR", amount = 5},
            classes = {"PALADIN"},
            description = "Increases target's Strength by 5"
        })
        
        gameAPI.addPlayerSpell({
            name = "Crusader Strike",
            school = "Physical",
            manaCost = 15,
            castTime = 0,
            cooldown = 4.5,
            range = 5,
            damage = 110,
            damageType = "Physical",
            classes = {"PALADIN"},
            description = "A strong melee attack that deals 110% weapon damage"
        })
        
        gameAPI.addPlayerSpell({
            name = "Avenger's Shield",
            school = "Holy",
            manaCost = 45,
            castTime = 0,
            cooldown = 15,
            range = 30,
            damage = 120,
            damageType = "Holy",
            chainTargets = 2,
            crowdControl = "Silence",
            duration = 3,
            classes = {"PALADIN"},
            description = "Throws shield that bounces to 2 additional enemies"
        })
        
        -- Register new abilities
        gameAPI.addPlayerAbility({
            name = "Lay on Hands",
            class = "PALADIN",
            cost = 0,
            castTime = 0,
            cooldown = 600,
            range = 40,
            healing = "100%", -- Full heal
            description = "Heals target to full health"
        })
        
        gameAPI.addPlayerAbility({
            name = "Divine Protection",
            class = "PALADIN",
            cost = 30,
            castTime = 0,
            cooldown = 300,
            duration = 8,
            damageReduction = 50,
            description = "Reduces damage taken by 50% for 8 seconds"
        })
        
        gameAPI.addPlayerAbility({
            name = "Hand of Freedom",
            class = "PALADIN",
            cost = 25,
            castTime = 0,
            cooldown = 25,
            range = 40,
            duration = 6,
            removesSnare = true,
            immuneToSnare = true,
            description = "Removes and prevents movement impairment"
        })
        
        gameAPI.addPlayerAbility({
            name = "Avenging Wrath",
            class = "PALADIN",
            cost = 0,
            castTime = 0,
            cooldown = 180,
            duration = 20,
            damageIncrease = 30,
            healingIncrease = 30,
            description = "Increases damage and healing by 30% for 20 seconds"
        })
        
        gameAPI.addPlayerAbility({
            name = "Guardian of Kings",
            class = "PALADIN",
            cost = 60,
            castTime = 0,
            cooldown = 300,
            duration = 8,
            healthIncrease = 40,
            damageReduction = 40,
            description = "Increases max health and reduces damage taken"
        })
    end
}
