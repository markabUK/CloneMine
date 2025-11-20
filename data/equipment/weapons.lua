-- Weapons Plugin
-- Demonstrates weapons with damage stats and special abilities

-- Weapon types
local WeaponType = {
    SWORD = 1,
    AXE = 2,
    BOW = 3,
    CROSSBOW = 4,
    TRIDENT = 5
}

-- Weapon materials
local WeaponMaterial = {
    WOODEN = {
        name = "Wooden",
        damage = 4,
        durability = 59,
        attackSpeed = 1.6
    },
    STONE = {
        name = "Stone",
        damage = 5,
        durability = 131,
        attackSpeed = 1.6
    },
    IRON = {
        name = "Iron",
        damage = 6,
        durability = 250,
        attackSpeed = 1.6
    },
    GOLD = {
        name = "Gold",
        damage = 4,
        durability = 32,
        attackSpeed = 1.6,
        enchantability = 22
    },
    DIAMOND = {
        name = "Diamond",
        damage = 7,
        durability = 1561,
        attackSpeed = 1.6,
        rarity = "rare"
    }
}

-- Weapon class
local Weapon = {}
Weapon.__index = Weapon

function Weapon:new(material, weaponType)
    local weapon = setmetatable({}, Weapon)
    weapon.material = material
    weapon.type = weaponType
    weapon.damage = WeaponMaterial[material].damage
    weapon.durability = WeaponMaterial[material].durability
    weapon.currentDurability = weapon.durability
    weapon.attackSpeed = WeaponMaterial[material].attackSpeed
    weapon.enchantments = {}
    weapon.criticalChance = 0.05  -- 5% base crit chance
    weapon.criticalMultiplier = 1.5
    return weapon
end

function Weapon:getDamage()
    local baseDamage = self.damage
    
    -- Apply enchantment bonuses
    if self.enchantments.SHARPNESS then
        baseDamage = baseDamage + (self.enchantments.SHARPNESS * 0.5)
    end
    
    return baseDamage
end

function Weapon:attack(target)
    local damage = self:getDamage()
    
    -- Check for critical hit
    if math.random() < self.criticalChance then
        damage = damage * self.criticalMultiplier
        log("Critical hit!")
    end
    
    -- Apply knockback
    if self.enchantments.KNOCKBACK then
        log("Knockback applied!")
    end
    
    -- Apply fire aspect
    if self.enchantments.FIRE_ASPECT then
        log("Target is on fire!")
    end
    
    -- Damage weapon durability
    self:damage(1)
    
    log("Dealt " .. damage .. " damage with " .. 
        self.material .. " " .. self:getTypeName())
    
    return damage
end

function Weapon:damage(amount)
    self.currentDurability = math.max(0, self.currentDurability - amount)
    if self.currentDurability == 0 then
        log("Weapon broke: " .. self.material .. " " .. self:getTypeName())
        return true
    end
    return false
end

function Weapon:repair(amount)
    self.currentDurability = math.min(self.durability, self.currentDurability + amount)
end

function Weapon:getDurabilityPercentage()
    return (self.currentDurability / self.durability) * 100
end

function Weapon:getTypeName()
    local types = {"Sword", "Axe", "Bow", "Crossbow", "Trident"}
    return types[self.type]
end

function Weapon:addEnchantment(name, level)
    self.enchantments[name] = level
    log("Added " .. name .. " " .. level .. " to weapon")
end

-- Player weapon inventory
local playerWeapons = {}
local equippedWeapon = nil

-- Create weapon collection
function createWeaponCollection()
    local weapons = {}
    
    for material, _ in pairs(WeaponMaterial) do
        weapons[material .. "_SWORD"] = Weapon:new(material, WeaponType.SWORD)
    end
    
    return weapons
end

-- Special weapons
local specialWeapons = {
    EXCALIBUR = {
        name = "Excalibur",
        damage = 12,
        durability = 9999,
        attackSpeed = 2.0,
        criticalChance = 0.25,
        rarity = "legendary",
        specialAbility = "Divine Strike"
    },
    DRAGON_SLAYER = {
        name = "Dragon Slayer",
        damage = 15,
        durability = 5000,
        attackSpeed = 1.2,
        bonusAgainstDragons = 3.0,
        rarity = "legendary",
        specialAbility = "Dragon's Bane"
    }
}

function onLoad()
    log("Weapons Plugin loaded!")
    
    playerWeapons = createWeaponCollection()
    
    log("Available weapon materials:")
    for material, props in pairs(WeaponMaterial) do
        log("  - " .. props.name .. " (Damage: " .. props.damage .. 
            ", Durability: " .. props.durability .. ")")
    end
    
    log("Special weapons: " .. countTable(specialWeapons))
end

function equipWeapon(weapon)
    equippedWeapon = weapon
    log("Equipped: " .. weapon.material .. " " .. weapon:getTypeName())
    log("  Damage: " .. weapon:getDamage())
    log("  Durability: " .. math.floor(weapon:getDurabilityPercentage()) .. "%")
end

function unequipWeapon()
    if equippedWeapon then
        log("Unequipped: " .. equippedWeapon.material .. " " .. equippedWeapon:getTypeName())
        equippedWeapon = nil
    end
end

function attackWithEquippedWeapon(target)
    if equippedWeapon then
        return equippedWeapon:attack(target)
    else
        log("No weapon equipped!")
        return 1  -- Base unarmed damage
    end
end

function getWeaponStatus()
    if equippedWeapon then
        log("=== Weapon Status ===")
        log("Weapon: " .. equippedWeapon.material .. " " .. equippedWeapon:getTypeName())
        log("Damage: " .. equippedWeapon:getDamage())
        log("Attack Speed: " .. equippedWeapon.attackSpeed)
        log("Durability: " .. math.floor(equippedWeapon:getDurabilityPercentage()) .. "%")
        
        if next(equippedWeapon.enchantments) then
            log("Enchantments:")
            for enchant, level in pairs(equippedWeapon.enchantments) do
                log("  - " .. enchant .. " " .. level)
            end
        end
    else
        log("No weapon equipped")
    end
end

-- Weapon enchantments
local weaponEnchantments = {
    SHARPNESS = "Increases damage",
    SMITE = "Extra damage to undead",
    BANE_OF_ARTHROPODS = "Extra damage to spiders",
    KNOCKBACK = "Increases knockback",
    FIRE_ASPECT = "Sets target on fire",
    LOOTING = "Increases drop rates",
    SWEEPING_EDGE = "Increases sweep attack damage",
    UNBREAKING = "Increases durability"
}

function countTable(t)
    local count = 0
    for _ in pairs(t) do
        count = count + 1
    end
    return count
end

-- Example combat scenario
function simulateCombat()
    log("=== Combat Simulation ===")
    
    -- Equip diamond sword
    local diamondSword = Weapon:new("DIAMOND", WeaponType.SWORD)
    diamondSword:addEnchantment("SHARPNESS", 5)
    diamondSword:addEnchantment("FIRE_ASPECT", 2)
    
    equipWeapon(diamondSword)
    
    -- Simulate 5 attacks
    for i = 1, 5 do
        log("Attack " .. i .. ":")
        attackWithEquippedWeapon("Enemy")
    end
    
    getWeaponStatus()
end
