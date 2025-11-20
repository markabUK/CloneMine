-- Example Plugin: Item Extension
-- This demonstrates how to add new items, weapons, and armor to the game

-- Add custom consumable items
function addCustomConsumables()
    local items = {
        {
            id = "MEGA_POTION",
            name = "Mega Health Potion",
            type = "Consumable",
            stackSize = 20,
            effect = {
                type = "HEAL",
                amount = 100,  -- Heals 100 HP instantly
                duration = 0
            },
            description = "A powerful potion that instantly restores 100 health",
            rarity = "Rare",
            value = 50
        },
        {
            id = "MANA_ELIXIR",
            name = "Elixir of Infinite Mana",
            type = "Consumable",
            stackSize = 5,
            effect = {
                type = "MANA_REGEN_BUFF",
                amount = 50,  -- +50 mana/sec
                duration = 30  -- 30 seconds
            },
            description = "Grants 50 mana regeneration per second for 30 seconds",
            rarity = "Epic",
            value = 100
        },
        {
            id = "STAT_FOOD",
            name = "Feast of Champions",
            type = "Consumable",
            stackSize = 10,
            effect = {
                type = "STAT_BUFF",
                stats = {
                    STR = 10,
                    DEX = 10,
                    CON = 10,
                    INT = 10,
                    WIS = 10,
                    CHA = 10
                },
                duration = 600  -- 10 minutes
            },
            description = "Increases all stats by 10 for 10 minutes",
            rarity = "Epic",
            value = 200
        }
    }
    
    for _, item in ipairs(items) do
        log("Registered custom consumable: " .. item.name)
    end
    
    return items
end

-- Add custom legendary weapons
function addCustomWeapons()
    local weapons = {
        {
            id = "FROSTMOURNE",
            name = "Frostmourne",
            type = "2H_SWORD",
            slot = "MAIN_HAND",
            damage = 30,
            stats = {
                STR = 50,
                INT = 30,
                armor = 20
            },
            specialAbilities = {
                "Soul Reaper",  -- Attacks steal 5% of target's max health
                "Frozen Soul",  -- 20% chance to freeze enemy for 3 seconds
                "Howling Blast"  -- Active ability: AOE frost damage
            },
            rarity = "Legendary",
            level = 20,
            restrictedTo = {"FIGHTER", "NECROMANCER"},
            twoHanded = true,
            description = "A cursed runeblade that hungers for souls. Once wielded by the Lich King himself."
        },
        {
            id = "STAFF_OF_DOMINANCE",
            name = "Staff of Dominance",
            type = "STAFF",
            slot = "MAIN_HAND",
            damage = 15,
            stats = {
                INT = 60,
                WIS = 40,
                spellPower = 100
            },
            specialAbilities = {
                "Mind Control",  -- Take control of an enemy for 10 seconds
                "Arcane Surge",  -- All spells cost 50% less mana for 15 seconds
                "Power Word: Dominate"  -- Ultimate: Control a boss-level enemy
            },
            rarity = "Legendary",
            level = 20,
            restrictedTo = {"WIZARD", "SORCERER", "MASTERMIND"},
            twoHanded = true,
            description = "This ancient staff pulses with arcane energy, granting the wielder dominion over minds."
        }
    }
    
    for _, weapon in ipairs(weapons) do
        log("Registered custom weapon: " .. weapon.name)
    end
    
    return weapons
end

-- Add custom armor sets
function addCustomArmor()
    local armorPieces = {
        -- Shadow Assassin Set (Leather)
        {
            id = "SHADOW_HOOD",
            name = "Shadow Assassin's Hood",
            type = "HELMET",
            armorType = "LEATHER",
            armor = 15,
            stats = {
                DEX = 20,
                CHA = 10,
                critChance = 0.05
            },
            set = "SHADOW_ASSASSIN",
            rarity = "Epic",
            level = 18,
            restrictedTo = {"RANGER", "WARLOCK"}
        },
        {
            id = "SHADOW_VEST",
            name = "Shadow Assassin's Vest",
            type = "CHEST",
            armorType = "LEATHER",
            armor = 30,
            stats = {
                DEX = 30,
                CHA = 15,
                critDamage = 0.25
            },
            set = "SHADOW_ASSASSIN",
            rarity = "Epic",
            level = 18,
            restrictedTo = {"RANGER", "WARLOCK"}
        },
        {
            id = "SHADOW_PANTS",
            name = "Shadow Assassin's Leggings",
            type = "LEGS",
            armorType = "LEATHER",
            armor = 25,
            stats = {
                DEX = 25,
                CHA = 12,
                speed = 0.10
            },
            set = "SHADOW_ASSASSIN",
            rarity = "Epic",
            level = 18,
            restrictedTo = {"RANGER", "WARLOCK"}
        },
        
        -- Archmage Set (Cloth)
        {
            id = "ARCHMAGE_CROWN",
            name = "Archmage's Crown",
            type = "HELMET",
            armorType = "CLOTH",
            armor = 8,
            stats = {
                INT = 35,
                WIS = 20,
                spellPower = 50,
                manaRegen = 10
            },
            set = "ARCHMAGE",
            rarity = "Legendary",
            level = 20,
            restrictedTo = {"WIZARD", "SORCERER", "NECROMANCER"}
        },
        {
            id = "ARCHMAGE_ROBE",
            name = "Archmage's Robes",
            type = "CHEST",
            armorType = "CLOTH",
            armor = 12,
            stats = {
                INT = 50,
                WIS = 30,
                spellPower = 100,
                spellCrit = 0.15
            },
            set = "ARCHMAGE",
            rarity = "Legendary",
            level = 20,
            restrictedTo = {"WIZARD", "SORCERER", "NECROMANCER"}
        }
    }
    
    for _, armor in ipairs(armorPieces) do
        log("Registered custom armor: " .. armor.name)
    end
    
    return armorPieces
end

-- Add custom jewelry
function addCustomJewelry()
    local jewelry = {
        {
            id = "RING_OF_POWER",
            name = "The One Ring of Power",
            type = "RING",
            stats = {
                STR = 20,
                DEX = 20,
                CON = 20,
                INT = 20,
                WIS = 20,
                CHA = 20,
                allDamage = 0.25,
                damageReduction = 0.25
            },
            specialAbility = "Invisibility",  -- Turn invisible for 30 seconds, 5 min cooldown
            curse = "Corruption",  -- Slowly drains health when worn
            rarity = "Legendary",
            level = 20,
            description = "One ring to rule them all... but at what cost?"
        },
        {
            id = "AMULET_OF_RESURRECTION",
            name = "Phoenix Amulet",
            type = "NECKLACE",
            stats = {
                CON = 30,
                WIS = 30,
                maxHealth = 500
            },
            specialAbility = "Auto-Resurrect",  -- Automatically resurrect at full health once per hour
            rarity = "Legendary",
            level = 18,
            description = "When you die, you are instantly resurrected with full health. Can only trigger once per hour."
        }
    }
    
    for _, item in ipairs(jewelry) do
        log("Registered custom jewelry: " .. item.name)
    end
    
    return jewelry
end

-- Initialize the extension
function onLoad()
    log("Loading Item Extension Example Plugin")
    
    -- Register custom items
    addCustomConsumables()
    addCustomWeapons()
    addCustomArmor()
    addCustomJewelry()
    
    log("Item Extension Example Plugin loaded successfully!")
end

-- Call onLoad when plugin is loaded
onLoad()
