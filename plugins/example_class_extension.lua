-- Example Plugin: Class Extension
-- This demonstrates how to extend existing classes with new abilities and spells

-- Extend the Wizard class with a new custom spell
function extendWizardClass()
    -- Add a custom spell to Wizard
    local customSpell = {
        name = "Arcane Meteor",
        school = "Arcane",
        level = 15,
        manaCost = 80,
        castTime = 3.0,
        cooldown = 60,
        damage = 200,
        aoeRadius = 10,
        description = "Summons a massive arcane meteor that crashes down, dealing heavy damage to all enemies in the area",
        effect = "Creates a targeting circle. After 3 seconds, meteor impacts dealing 200 damage to all enemies within 10 yards"
    }
    
    -- Register the custom spell with the game
    log("Registered custom spell: Arcane Meteor for Wizard class")
    
    return customSpell
end

-- Add a new passive ability to Necromancer
function extendNecromancerClass()
    local customAbility = {
        name = "Soul Reaper",
        type = "Passive",
        level = 18,
        description = "When an enemy dies within 30 yards, you gain 5% of your max health and mana",
        effect = function(player, enemy)
            -- Heal player for 5% of max health
            local healAmount = player.maxHealth * 0.05
            player.health = math.min(player.health + healAmount, player.maxHealth)
            
            -- Restore 5% of max mana
            local manaAmount = player.maxMana * 0.05
            player.mana = math.min(player.mana + manaAmount, player.maxMana)
            
            log("Soul Reaper triggered: Gained " .. healAmount .. " health and " .. manaAmount .. " mana")
        end
    }
    
    log("Registered custom ability: Soul Reaper for Necromancer class")
    
    return customAbility
end

-- Add a new talent to Druid's shapeshifting
function extendDruidShapeshifting()
    local customForm = {
        name = "PHOENIX",
        displayName = "Phoenix Form",
        level = 20,
        duration = 0,  -- Permanent until unshifted
        cooldown = 0,
        statMultipliers = {
            health = 1.2,
            damage = 1.8,
            armor = 1.0,
            spellPower = 2.0,
            speed = 1.5
        },
        abilities = {
            "Flight",
            "Fire Breath",
            "Rebirth"  -- Auto-resurrect once every 10 minutes
        },
        canCast = true,  -- Can cast spells in this form
        description = "Transform into a legendary phoenix with immense fire power and the ability to resurrect from death once per cooldown"
    }
    
    log("Registered custom shapeshift form: Phoenix for Druid class")
    
    return customForm
end

-- Example: Add new equipment set bonus for Fighter
function addCustomEquipmentSet()
    local berserkerSet = {
        name = "Berserker's Rage",
        pieces = {
            "Berserker Helmet",
            "Berserker Chest",
            "Berserker Legs",
            "Berserker Boots",
            "Berserker Gauntlets"
        },
        bonuses = {
            ["2_piece"] = {
                description = "+10% critical strike chance",
                stats = { critChance = 0.10 }
            },
            ["4_piece"] = {
                description = "+20% attack speed, -10% damage taken",
                stats = { attackSpeed = 0.20, damageReduction = 0.10 }
            },
            ["5_piece"] = {
                description = "Entering combat grants Berserk: +50% damage, +30% attack speed for 15 seconds",
                effect = "BERSERK_PROC"
            }
        },
        restrictedTo = {"FIGHTER"}
    }
    
    log("Registered custom equipment set: Berserker's Rage for Fighter class")
    
    return berserkerSet
end

-- Initialize the extension
function onLoad()
    log("Loading Class Extension Example Plugin")
    
    -- Register custom content
    extendWizardClass()
    extendNecromancerClass()
    extendDruidShapeshifting()
    addCustomEquipmentSet()
    
    log("Class Extension Example Plugin loaded successfully!")
end

-- Call onLoad when plugin is loaded
onLoad()
