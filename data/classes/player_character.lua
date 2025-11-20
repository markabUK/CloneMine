-- Player Character Plugin
-- Demonstrates player stats, attributes, and progression system

-- Player stats
local PlayerStats = {
    -- Health
    maxHealth = 20,
    currentHealth = 20,
    healthRegen = 0.1,  -- Health per second
    
    -- Hunger
    maxHunger = 20,
    currentHunger = 20,
    hungerDepletion = 0.05,  -- Hunger per second while moving
    
    -- Stamina
    maxStamina = 100,
    currentStamina = 100,
    staminaRegen = 5,  -- Stamina per second
    
    -- Movement
    walkSpeed = 4.3,
    sprintSpeed = 5.6,
    jumpHeight = 1.25,
    
    -- Combat
    baseDamage = 1,
    attackRange = 3,
    
    -- Experience
    level = 1,
    experience = 0,
    experienceToNextLevel = 100
}

-- Player attributes (can be modified by armor, items, etc.)
local PlayerAttributes = {
    strength = 10,      -- Increases melee damage
    defense = 10,       -- Reduces incoming damage
    agility = 10,       -- Affects movement speed and dodge chance
    vitality = 10,      -- Increases max health
    intelligence = 10,  -- Affects magic and enchanting
    luck = 10          -- Affects drops and critical hits
}

-- Player inventory
local PlayerInventory = {
    maxSlots = 36,
    hotbarSize = 9,
    items = {},
    armor = {
        helmet = nil,
        chestplate = nil,
        leggings = nil,
        boots = nil
    },
    offhand = nil
}

-- Player status effects
local StatusEffects = {
    regeneration = {active = false, duration = 0, amplifier = 0},
    strength = {active = false, duration = 0, amplifier = 0},
    speed = {active = false, duration = 0, amplifier = 0},
    jump_boost = {active = false, duration = 0, amplifier = 0},
    resistance = {active = false, duration = 0, amplifier = 0},
    fire_resistance = {active = false, duration = 0, amplifier = 0},
    water_breathing = {active = false, duration = 0, amplifier = 0},
    invisibility = {active = false, duration = 0, amplifier = 0},
    poison = {active = false, duration = 0, amplifier = 0},
    weakness = {active = false, duration = 0, amplifier = 0}
}

function onLoad()
    log("Player Character Plugin loaded!")
    log("=== Player Stats ===")
    log("Health: " .. PlayerStats.currentHealth .. "/" .. PlayerStats.maxHealth)
    log("Hunger: " .. PlayerStats.currentHunger .. "/" .. PlayerStats.maxHunger)
    log("Level: " .. PlayerStats.level)
    log("Experience: " .. PlayerStats.experience .. "/" .. PlayerStats.experienceToNextLevel)
end

function onUpdate(deltaTime)
    -- Update player stats
    updateHealth(deltaTime)
    updateHunger(deltaTime)
    updateStamina(deltaTime)
    updateStatusEffects(deltaTime)
end

function updateHealth(deltaTime)
    -- Regenerate health if hunger is high enough
    if PlayerStats.currentHunger > 18 and PlayerStats.currentHealth < PlayerStats.maxHealth then
        PlayerStats.currentHealth = math.min(
            PlayerStats.maxHealth,
            PlayerStats.currentHealth + PlayerStats.healthRegen * deltaTime
        )
    end
    
    -- Apply regeneration effect
    if StatusEffects.regeneration.active then
        local regenAmount = (1 + StatusEffects.regeneration.amplifier) * 0.5 * deltaTime
        PlayerStats.currentHealth = math.min(
            PlayerStats.maxHealth,
            PlayerStats.currentHealth + regenAmount
        )
    end
end

function updateHunger(deltaTime)
    -- Deplete hunger over time (faster when sprinting)
    PlayerStats.currentHunger = math.max(
        0,
        PlayerStats.currentHunger - PlayerStats.hungerDepletion * deltaTime
    )
    
    -- Take damage if hunger is at 0
    if PlayerStats.currentHunger == 0 then
        takeDamage(1 * deltaTime)
    end
end

function updateStamina(deltaTime)
    -- Regenerate stamina when not sprinting
    PlayerStats.currentStamina = math.min(
        PlayerStats.maxStamina,
        PlayerStats.currentStamina + PlayerStats.staminaRegen * deltaTime
    )
end

function updateStatusEffects(deltaTime)
    for effectName, effect in pairs(StatusEffects) do
        if effect.active then
            effect.duration = effect.duration - deltaTime
            if effect.duration <= 0 then
                effect.active = false
                log("Status effect expired: " .. effectName)
            end
        end
    end
end

-- Damage calculation
function takeDamage(amount)
    -- Apply defense reduction
    local defenseMultiplier = 1 - (PlayerAttributes.defense / 100)
    local actualDamage = amount * defenseMultiplier
    
    -- Apply resistance effect
    if StatusEffects.resistance.active then
        local resistance = (1 + StatusEffects.resistance.amplifier) * 0.2
        actualDamage = actualDamage * (1 - resistance)
    end
    
    PlayerStats.currentHealth = math.max(0, PlayerStats.currentHealth - actualDamage)
    log("Player took " .. string.format("%.1f", actualDamage) .. " damage")
    
    if PlayerStats.currentHealth == 0 then
        log("Player died!")
    end
end

-- Healing
function heal(amount)
    PlayerStats.currentHealth = math.min(
        PlayerStats.maxHealth,
        PlayerStats.currentHealth + amount
    )
    log("Player healed " .. amount .. " health")
end

-- Eating food
function eat(foodValue)
    PlayerStats.currentHunger = math.min(
        PlayerStats.maxHunger,
        PlayerStats.currentHunger + foodValue
    )
    log("Player ate food (+" .. foodValue .. " hunger)")
end

-- Experience and leveling
function addExperience(amount)
    PlayerStats.experience = PlayerStats.experience + amount
    log("Gained " .. amount .. " experience")
    
    while PlayerStats.experience >= PlayerStats.experienceToNextLevel do
        levelUp()
    end
end

function levelUp()
    PlayerStats.experience = PlayerStats.experience - PlayerStats.experienceToNextLevel
    PlayerStats.level = PlayerStats.level + 1
    PlayerStats.experienceToNextLevel = math.floor(PlayerStats.experienceToNextLevel * 1.1)
    
    log("=== LEVEL UP! ===")
    log("Now level " .. PlayerStats.level)
    
    -- Grant attribute points
    grantAttributePoints(3)
end

function grantAttributePoints(points)
    log("Gained " .. points .. " attribute points to distribute")
end

function increaseAttribute(attributeName, amount)
    if PlayerAttributes[attributeName] then
        PlayerAttributes[attributeName] = PlayerAttributes[attributeName] + amount
        log(attributeName .. " increased to " .. PlayerAttributes[attributeName])
        
        -- Apply attribute effects
        applyAttributeEffects(attributeName)
    end
end

function applyAttributeEffects(attributeName)
    if attributeName == "vitality" then
        -- Increase max health
        PlayerStats.maxHealth = 20 + (PlayerAttributes.vitality - 10) * 2
    elseif attributeName == "agility" then
        -- Increase movement speed
        PlayerStats.walkSpeed = 4.3 + (PlayerAttributes.agility - 10) * 0.1
    end
end

-- Status effect application
function applyStatusEffect(effectName, duration, amplifier)
    if StatusEffects[effectName] then
        StatusEffects[effectName].active = true
        StatusEffects[effectName].duration = duration
        StatusEffects[effectName].amplifier = amplifier or 0
        log("Applied " .. effectName .. " for " .. duration .. " seconds")
    end
end

-- Get player stats display
function displayPlayerStats()
    log("=== Player Character ===")
    log("Level: " .. PlayerStats.level .. " (XP: " .. PlayerStats.experience .. 
        "/" .. PlayerStats.experienceToNextLevel .. ")")
    log("")
    log("Health: " .. string.format("%.1f", PlayerStats.currentHealth) .. 
        "/" .. PlayerStats.maxHealth)
    log("Hunger: " .. string.format("%.1f", PlayerStats.currentHunger) .. 
        "/" .. PlayerStats.maxHunger)
    log("Stamina: " .. string.format("%.1f", PlayerStats.currentStamina) .. 
        "/" .. PlayerStats.maxStamina)
    log("")
    log("=== Attributes ===")
    for attr, value in pairs(PlayerAttributes) do
        log(attr .. ": " .. value)
    end
    log("")
    log("Active Effects:")
    for effectName, effect in pairs(StatusEffects) do
        if effect.active then
            log("  - " .. effectName .. " (" .. 
                string.format("%.1f", effect.duration) .. "s remaining)")
        end
    end
end

-- Example combat scenario
function examplePlayerScenario()
    log("\n=== Player Scenario Demo ===\n")
    
    -- Show initial stats
    displayPlayerStats()
    
    -- Take damage
    log("\n--- Combat ---")
    takeDamage(5)
    
    -- Heal
    heal(3)
    
    -- Eat food
    eat(6)
    
    -- Gain experience
    log("\n--- Experience Gain ---")
    addExperience(250)
    
    -- Apply buffs
    log("\n--- Buffs ---")
    applyStatusEffect("strength", 30, 1)
    applyStatusEffect("regeneration", 45, 0)
    
    -- Increase attributes
    log("\n--- Attribute Improvement ---")
    increaseAttribute("strength", 2)
    increaseAttribute("vitality", 3)
    
    -- Final stats
    log("")
    displayPlayerStats()
end
