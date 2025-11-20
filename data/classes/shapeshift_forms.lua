-- Shapeshift Forms Plugin
-- DRUID-ONLY shapeshifting system with various animal forms
-- All shapeshifts last until you unshift or are killed
-- Ghost form when dead removes all shapeshifts

-- Shapeshift Forms
local ShapeshiftForm = {
    -- Early Forms
    BEAR = {
        name = "Bear Form",
        requiredLevel = 2,
        requiredClass = "DRUID",
        cost = 20,
        duration = 0,  -- 0 = until cancelled
        description = "Transform into a mighty bear",
        statsMultiplier = {
            STR = 1.5,
            CON = 1.8,
            armor = 2.0
        },
        abilities = {"Maul", "Swipe", "Growl"},
        restrictions = {
            canCast = false,
            canHeal = false
        },
        model = "bear",
        movementSpeed = 0.9
    },
    
    CAT = {
        name = "Cat Form",
        requiredLevel = 7,
        requiredClass = "DRUID",
        cost = 20,
        duration = 0,
        description = "Transform into an agile cat",
        statsMultiplier = {
            DEX = 1.8,
            STR = 1.3
        },
        abilities = {"Claw", "Pounce", "Rake", "Rip"},
        restrictions = {
            canCast = false,
            canHeal = false
        },
        model = "cat",
        movementSpeed = 1.3,
        stealth = true
    },
    
    AQUATIC = {
        name = "Aquatic Form",
        requiredLevel = 4,
        requiredClass = "DRUID",
        cost = 10,
        duration = 0,
        description = "Transform into a sea creature",
        statsMultiplier = {},
        abilities = {},
        restrictions = {
            canCast = false,
            canHeal = false,
            waterOnly = true
        },
        model = "seal",
        movementSpeed = 2.0,
        waterBreathing = true
    },
    
    TRAVEL = {
        name = "Travel Form",
        requiredLevel = 10,
        requiredClass = "DRUID",
        cost = 15,
        duration = 0,
        description = "Transform into a swift animal",
        statsMultiplier = {},
        abilities = {},
        restrictions = {
            canCast = false,
            canHeal = false,
            noCombat = true
        },
        model = "cheetah",
        movementSpeed = 1.8
    },
    
    -- Mid-Level Forms
    LION = {
        name = "Lion Form",
        requiredLevel = 12,
        requiredClass = "DRUID",
        cost = 30,
        duration = 0,
        description = "Transform into a fierce lion",
        statsMultiplier = {
            STR = 1.7,
            DEX = 1.5,
            CON = 1.3
        },
        abilities = {"Bite", "Roar", "Pounce", "Savage Bite"},
        restrictions = {
            canCast = false,
            canHeal = false
        },
        model = "lion",
        movementSpeed = 1.2,
        pack = {
            bonusWithAllies = 1.15,
            leadershipAura = true
        }
    },
    
    DIRE_BEAR = {
        name = "Dire Bear Form",
        requiredLevel = 15,
        requiredClass = "DRUID",
        cost = 35,
        duration = 0,
        description = "Transform into a massive dire bear",
        statsMultiplier = {
            STR = 1.8,
            CON = 2.2,
            armor = 2.5
        },
        abilities = {"Maul", "Swipe", "Bash", "Enrage", "Demoralizing Roar"},
        restrictions = {
            canCast = false,
            canHeal = false
        },
        model = "dire_bear",
        movementSpeed = 0.85,
        threatMultiplier = 2.0
    },
    
    MOONKIN = {
        name = "Moonkin Form",
        requiredLevel = 16,
        requiredClass = "DRUID",
        cost = 40,
        duration = 0,
        description = "Transform into a moonkin, can cast spells",
        statsMultiplier = {
            INT = 1.5,
            spellpower = 1.3,
            armor = 1.5
        },
        abilities = {},  -- Can cast normal spells
        restrictions = {
            canCast = true,  -- Can cast spells!
            canHeal = true,
            spellSchools = {"ARCANE", "NATURE"}
        },
        model = "moonkin",
        movementSpeed = 1.0,
        aura = {
            type = "spellCrit",
            bonus = 0.05,
            radius = 30
        }
    },
    
    -- High-Level Forms
    WYVERN = {
        name = "Wyvern Form",
        requiredLevel = 18,
        requiredClass = "DRUID",
        cost = 50,
        duration = 0,
        description = "Transform into a flying wyvern",
        statsMultiplier = {
            STR = 1.6,
            DEX = 1.9,
            CON = 1.4
        },
        abilities = {"Tail Lash", "Aerial Strike", "Poison Sting"},
        restrictions = {
            canCast = false,
            canHeal = false
        },
        model = "wyvern",
        movementSpeed = 1.5,
        canFly = true,
        flightSpeed = 2.5
    },
    
    TREANT = {
        name = "Tree of Life",
        requiredLevel = 19,
        requiredClass = "DRUID",
        cost = 60,
        duration = 0,
        description = "Transform into a tree of life, ultimate healing form",
        statsMultiplier = {
            WIS = 1.8,
            healingPower = 1.5,
            CON = 1.6
        },
        abilities = {},  -- Can cast healing spells
        restrictions = {
            canCast = true,
            canHeal = true,
            spellSchools = {"HOLY", "NATURE"}
        },
        model = "treant",
        movementSpeed = 0.7,
        aura = {
            type = "healing",
            bonus = 0.15,
            radius = 40
        },
        healingBonus = 0.3
    },
    
    -- Epic Dragon Forms (DRUID EXCLUSIVE)
    DRAGON_WHELP = {
        name = "Dragon Whelp",
        requiredLevel = 14,
        requiredClass = "DRUID",  -- ONLY DRUIDS can become dragons
        cost = 70,
        duration = 0,  -- Lasts until unshifted or killed
        description = "Transform into a young dragon (DRUID ONLY)",
        statsMultiplier = {
            STR = 2.0,
            DEX = 1.7,
            CON = 1.9,
            INT = 1.5,
            armor = 2.0
        },
        abilities = {"Flame Breath", "Wing Buffet", "Tail Swipe", "Roar"},
        restrictions = {
            canCast = true,
            canHeal = false,
            spellSchools = {"FIRE", "NATURE"}
        },
        model = "dragon_whelp",
        movementSpeed = 1.2,
        canFly = true,
        flightSpeed = 3.0,
        breathWeapon = {
            damage = 60,
            cooldown = 12,
            range = 20,
            type = "fire"
        }
    },
    
    DRAKE = {
        name = "Drake Form",
        requiredLevel = 17,
        requiredClass = "DRUID",  -- ONLY DRUIDS can become dragons
        cost = 80,
        duration = 0,  -- Lasts until unshifted or killed
        description = "Transform into a powerful drake (DRUID ONLY)",
        statsMultiplier = {
            STR = 2.3,
            DEX = 1.8,
            CON = 2.2,
            INT = 1.6,
            WIS = 1.4,
            armor = 2.5
        },
        abilities = {"Frost Breath", "Cleave", "Wing Buffet", "Frightful Presence"},
        restrictions = {
            canCast = true,
            canHeal = true,
            spellSchools = {"FROST", "NATURE", "ARCANE"}
        },
        model = "drake",
        movementSpeed = 1.3,
        canFly = true,
        flightSpeed = 3.5,
        breathWeapon = {
            damage = 90,
            cooldown = 10,
            range = 25,
            type = "frost",
            slow = 0.8
        },
        fearAura = {
            radius = 15,
            chance = 0.1
        }
    },
    
    DRAGON = {
        name = "Dragon Form",
        requiredLevel = 20,  -- Ultimate form at max level
        requiredClass = "DRUID",  -- ONLY DRUIDS can become dragons
        cost = 100,
        duration = 0,  -- Lasts until unshifted or killed
        description = "Transform into a mighty dragon - ultimate form (DRUID ONLY)",
        statsMultiplier = {
            STR = 2.5,
            DEX = 2.0,
            CON = 2.5,
            INT = 1.8,
            WIS = 1.6,
            armor = 3.0,
            spellpower = 1.5
        },
        abilities = {
            "Ancient Breath", "Tail Swipe", "Wing Storm", 
            "Terrifying Roar", "Draconic Might"
        },
        restrictions = {
            canCast = true,
            canHeal = true,
            spellSchools = {"FIRE", "FROST", "ARCANE", "NATURE"}
        },
        model = "dragon",
        movementSpeed = 1.4,
        canFly = true,
        flightSpeed = 4.0,
        breathWeapon = {
            damage = 150,
            cooldown = 8,
            range = 30,
            type = "elemental",  -- Cycles fire/frost/nature
            aoe = true,
            radius = 10
        },
        fearAura = {
            radius = 25,
            chance = 0.25
        },
        legendary = true
    }
}

-- Current form state
local currentForm = nil
local formStartTime = 0

-- Shapeshift into form
function shapeshift(formName, playerClass, playerLevel)
    local form = ShapeshiftForm[formName]
    
    if not form then
        log("Unknown shapeshift form: " .. formName)
        return false
    end
    
    -- Check class requirement
    if playerClass.name ~= form.requiredClass then
        log("Only " .. form.requiredClass .. " can use " .. form.name)
        return false
    end
    
    -- Check level requirement
    if playerLevel < form.requiredLevel then
        log(form.name .. " requires level " .. form.requiredLevel)
        return false
    end
    
    -- Check if on cooldown
    -- (cooldown tracking would go here)
    
    -- Cancel current form if any
    if currentForm then
        cancelShapeshift()
    end
    
    -- Apply form
    currentForm = form
    formStartTime = os.time()
    
    log("=== Shapeshifted into " .. form.name .. " ===")
    log(form.description)
    
    if form.duration > 0 then
        log("Duration: " .. form.duration .. " seconds")
    else
        log("Duration: Until cancelled")
    end
    
    -- Log stat changes
    if next(form.statsMultiplier) then
        log("Stat bonuses:")
        for stat, mult in pairs(form.statsMultiplier) do
            log("  " .. stat .. ": x" .. mult)
        end
    end
    
    -- Log abilities
    if #form.abilities > 0 then
        log("Form abilities: " .. table.concat(form.abilities, ", "))
    end
    
    -- Log special features
    if form.canFly then
        log("Can fly! Flight speed: x" .. form.flightSpeed)
    end
    
    if form.stealth then
        log("Can use stealth")
    end
    
    if form.waterBreathing then
        log("Can breathe underwater")
    end
    
    if form.breathWeapon then
        log("Breath weapon: " .. form.breathWeapon.damage .. " " .. 
            form.breathWeapon.type .. " damage")
    end
    
    return true
end

-- Cancel shapeshift
function cancelShapeshift()
    if not currentForm then
        log("Not in any form")
        return false
    end
    
    log("Cancelled " .. currentForm.name)
    currentForm = nil
    formStartTime = 0
    return true
end

-- Get current form
function getCurrentForm()
    return currentForm
end

-- Check if form has expired
function updateShapeshift(currentTime)
    if currentForm and currentForm.duration > 0 then
        local elapsed = currentTime - formStartTime
        if elapsed >= currentForm.duration then
            log(currentForm.name .. " has expired")
            currentForm = nil
            formStartTime = 0
        end
    end
end

-- Get stat multiplier for current form
function getFormStatMultiplier(stat)
    if currentForm and currentForm.statsMultiplier[stat] then
        return currentForm.statsMultiplier[stat]
    end
    return 1.0
end

-- Check if can cast spells in current form
function canCastInForm()
    if not currentForm then
        return true  -- Normal form, can cast
    end
    return currentForm.restrictions.canCast or false
end

-- Check if can heal in current form
function canHealInForm()
    if not currentForm then
        return true
    end
    return currentForm.restrictions.canHeal or false
end

-- List all available forms for level
function listAvailableForms(playerLevel)
    log("=== Available Shapeshift Forms (Level " .. playerLevel .. ") ===")
    
    for formName, form in pairs(ShapeshiftForm) do
        if playerLevel >= form.requiredLevel then
            local tag = form.legendary and " [LEGENDARY]" or ""
            local cd = form.cooldown and (" (CD: " .. form.cooldown .. "s)") or ""
            log("  " .. form.name .. " (Lvl " .. form.requiredLevel .. ")" .. tag .. cd)
            log("    " .. form.description)
        end
    end
end

function onLoad()
    log("Shapeshift Forms Plugin loaded!")
    
    local formCount = 0
    local formsByLevel = {}
    
    for _, form in pairs(ShapeshiftForm) do
        formCount = formCount + 1
        local levelRange = math.floor(form.requiredLevel / 10) * 10
        formsByLevel[levelRange] = (formsByLevel[levelRange] or 0) + 1
    end
    
    log("Total shapeshift forms: " .. formCount)
    log("\nForms by level range:")
    for level, count in pairs(formsByLevel) do
        log("  Level " .. level .. "-" .. (level + 9) .. ": " .. count .. " forms")
    end
    
    log("\nEpic Forms (with cooldowns):")
    for formName, form in pairs(ShapeshiftForm) do
        if form.cooldown and form.cooldown > 0 then
            log("  " .. form.name .. " (Level " .. form.requiredLevel .. 
                ", " .. form.duration .. "s duration, " .. form.cooldown .. "s CD)")
        end
    end
end

function onUpdate(deltaTime)
    updateShapeshift(os.time())
end

-- Export for other plugins
_G.ShapeshiftForm = ShapeshiftForm
_G.shapeshift = shapeshift
_G.cancelShapeshift = cancelShapeshift
_G.getCurrentForm = getCurrentForm
_G.getFormStatMultiplier = getFormStatMultiplier
_G.canCastInForm = canCastInForm
_G.canHealInForm = canHealInForm
_G.listAvailableForms = listAvailableForms
