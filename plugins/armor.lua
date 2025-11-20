-- Armor Plugin
-- Demonstrates armor system with protection values

-- Armor slots
local ArmorSlot = {
    HELMET = 1,
    CHESTPLATE = 2,
    LEGGINGS = 3,
    BOOTS = 4
}

-- Armor materials
local ArmorMaterial = {
    LEATHER = {
        name = "Leather",
        protection = {2, 3, 2, 1},  -- helmet, chest, legs, boots
        durability = {55, 80, 75, 65},
        enchantability = 15
    },
    CHAINMAIL = {
        name = "Chainmail",
        protection = {2, 5, 4, 1},
        durability = {165, 240, 225, 195},
        enchantability = 12
    },
    IRON = {
        name = "Iron",
        protection = {2, 6, 5, 2},
        durability = {165, 240, 225, 195},
        enchantability = 9
    },
    GOLD = {
        name = "Gold",
        protection = {2, 5, 3, 1},
        durability = {77, 112, 105, 91},
        enchantability = 25
    },
    DIAMOND = {
        name = "Diamond",
        protection = {3, 8, 6, 3},
        durability = {363, 528, 495, 429},
        enchantability = 10,
        rarity = "rare"
    }
}

-- Armor class
local Armor = {}
Armor.__index = Armor

function Armor:new(material, slot)
    local armor = setmetatable({}, Armor)
    armor.material = material
    armor.slot = slot
    armor.protection = ArmorMaterial[material].protection[slot]
    armor.durability = ArmorMaterial[material].durability[slot]
    armor.currentDurability = armor.durability
    armor.enchantments = {}
    return armor
end

function Armor:getProtection()
    return self.protection
end

function Armor:damage(amount)
    self.currentDurability = math.max(0, self.currentDurability - amount)
    if self.currentDurability == 0 then
        log("Armor piece broke: " .. self.material .. " " .. self:getSlotName())
        return true  -- Armor broke
    end
    return false
end

function Armor:repair(amount)
    self.currentDurability = math.min(self.durability, self.currentDurability + amount)
end

function Armor:getDurabilityPercentage()
    return (self.currentDurability / self.durability) * 100
end

function Armor:getSlotName()
    local slots = {"Helmet", "Chestplate", "Leggings", "Boots"}
    return slots[self.slot]
end

function Armor:addEnchantment(name, level)
    self.enchantments[name] = level
end

-- Player armor inventory
local playerArmor = {
    [ArmorSlot.HELMET] = nil,
    [ArmorSlot.CHESTPLATE] = nil,
    [ArmorSlot.LEGGINGS] = nil,
    [ArmorSlot.BOOTS] = nil
}

-- Armor sets (for quick equipping)
local armorSets = {}

function createArmorSet(material)
    return {
        helmet = Armor:new(material, ArmorSlot.HELMET),
        chestplate = Armor:new(material, ArmorSlot.CHESTPLATE),
        leggings = Armor:new(material, ArmorSlot.LEGGINGS),
        boots = Armor:new(material, ArmorSlot.BOOTS)
    }
end

function onLoad()
    log("Armor Plugin loaded!")
    
    -- Create example armor sets
    armorSets["LEATHER"] = createArmorSet("LEATHER")
    armorSets["IRON"] = createArmorSet("IRON")
    armorSets["DIAMOND"] = createArmorSet("DIAMOND")
    
    log("Available armor materials:")
    for material, props in pairs(ArmorMaterial) do
        local totalProtection = 0
        for _, p in ipairs(props.protection) do
            totalProtection = totalProtection + p
        end
        log("  - " .. props.name .. " (Total Protection: " .. totalProtection .. ")")
    end
end

function equipArmor(slot, armor)
    playerArmor[slot] = armor
    log("Equipped: " .. armor.material .. " " .. armor:getSlotName())
end

function unequipArmor(slot)
    if playerArmor[slot] then
        log("Unequipped: " .. playerArmor[slot].material .. " " .. playerArmor[slot]:getSlotName())
        playerArmor[slot] = nil
    end
end

function getTotalProtection()
    local total = 0
    for _, armor in pairs(playerArmor) do
        if armor then
            total = total + armor:getProtection()
        end
    end
    return total
end

function damageAllArmor(amount)
    for slot, armor in pairs(playerArmor) do
        if armor then
            if armor:damage(amount) then
                -- Armor broke
                playerArmor[slot] = nil
            end
        end
    end
end

function getArmorStatus()
    log("=== Armor Status ===")
    log("Total Protection: " .. getTotalProtection())
    for slot, armor in pairs(playerArmor) do
        if armor then
            log(armor:getSlotName() .. ": " .. 
                armor.material .. " (" .. 
                math.floor(armor:getDurabilityPercentage()) .. "% durability)")
        end
    end
end

-- Example enchantments
local armorEnchantments = {
    PROTECTION = "Reduces damage from all sources",
    FIRE_PROTECTION = "Reduces fire damage",
    BLAST_PROTECTION = "Reduces explosion damage",
    PROJECTILE_PROTECTION = "Reduces projectile damage",
    FEATHER_FALLING = "Reduces fall damage (boots only)",
    THORNS = "Damages attackers",
    UNBREAKING = "Increases durability"
}
