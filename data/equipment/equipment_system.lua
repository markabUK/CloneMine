-- Equipment System Plugin
-- Implements D&D-style equipment with slots and class restrictions

-- Equipment Slots
local EquipmentSlot = {
    -- Weapons
    MAIN_HAND = "mainHand",
    OFF_HAND = "offHand",
    
    -- Armor
    HELMET = "helmet",
    SHOULDER = "shoulder",
    CHEST = "chest",
    BELT = "belt",
    BRACER = "bracer",
    GLOVES = "gloves",
    LEGS = "legs",
    BOOTS = "boots",
    
    -- Jewelry
    NECKLACE = "necklace",
    RING1 = "ring1",
    RING2 = "ring2"
}

-- Weapon Types
local WeaponType = {
    -- 1-Handed
    SWORD_1H = {name = "Sword (1H)", hands = 1, damage = 8, type = "sword"},
    AXE_1H = {name = "Axe (1H)", hands = 1, damage = 9, type = "axe"},
    MACE_1H = {name = "Mace (1H)", hands = 1, damage = 7, type = "mace"},
    DAGGER = {name = "Dagger", hands = 1, damage = 5, type = "dagger"},
    WAND = {name = "Wand", hands = 1, damage = 6, spellpower = 10, type = "wand"},
    
    -- 2-Handed
    SWORD_2H = {name = "Greatsword (2H)", hands = 2, damage = 15, type = "sword"},
    AXE_2H = {name = "Battleaxe (2H)", hands = 2, damage = 16, type = "axe"},
    STAFF = {name = "Staff (2H)", hands = 2, damage = 8, spellpower = 25, type = "staff"},
    BOW = {name = "Bow (2H)", hands = 2, damage = 12, range = true, type = "bow"},
    CROSSBOW = {name = "Crossbow (2H)", hands = 2, damage = 14, range = true, type = "crossbow"},
    SPEAR = {name = "Spear (2H)", hands = 2, damage = 11, type = "spear"},
    
    -- Shield (off-hand only)
    SHIELD = {name = "Shield", hands = 1, armor = 5, block = true, type = "shield"}
}

-- Armor Types
local ArmorType = {
    CLOTH = "cloth",
    LEATHER = "leather",
    MAIL = "mail",
    PLATE = "plate"
}

-- Item class
local Item = {}
Item.__index = Item

function Item:new(props)
    local item = setmetatable({}, Item)
    item.name = props.name
    item.slot = props.slot
    item.armorType = props.armorType
    item.armor = props.armor or 0
    item.stats = props.stats or {}
    item.requiredLevel = props.requiredLevel or 1
    item.quality = props.quality or "common"
    return item
end

-- Weapon class
local Weapon = {}
Weapon.__index = Weapon

function Weapon:new(weaponType, props)
    local weapon = setmetatable({}, Weapon)
    weapon.weaponType = weaponType
    weapon.name = props.name
    weapon.damage = weaponType.damage + (props.damageBonus or 0)
    weapon.hands = weaponType.hands
    weapon.type = weaponType.type
    weapon.spellpower = weaponType.spellpower or 0
    weapon.stats = props.stats or {}
    weapon.requiredLevel = props.requiredLevel or 1
    weapon.quality = props.quality or "common"
    weapon.range = weaponType.range or false
    weapon.block = weaponType.block or false
    return weapon
end

-- Equipment inventory
local EquippedItems = {
    mainHand = nil,
    offHand = nil,
    helmet = nil,
    shoulder = nil,
    chest = nil,
    belt = nil,
    bracer = nil,
    gloves = nil,
    legs = nil,
    boots = nil,
    necklace = nil,
    ring1 = nil,
    ring2 = nil
}

-- Example weapons
local ExampleWeapons = {
    -- Wizard weapons
    apprenticeWand = Weapon:new(WeaponType.WAND, {
        name = "Apprentice's Wand",
        stats = {INT = 2},
        requiredLevel = 1
    }),
    
    archmageStaff = Weapon:new(WeaponType.STAFF, {
        name = "Archmage's Staff",
        damageBonus = 5,
        stats = {INT = 10, spellpower = 50},
        requiredLevel = 20,
        quality = "epic"
    }),
    
    -- Fighter weapons
    ironSword = Weapon:new(WeaponType.SWORD_1H, {
        name = "Iron Sword",
        damageBonus = 2,
        stats = {STR = 3},
        requiredLevel = 5
    }),
    
    berserkerAxe = Weapon:new(WeaponType.AXE_2H, {
        name = "Berserker's Axe",
        damageBonus = 8,
        stats = {STR = 12, critChance = 5},
        requiredLevel = 15,
        quality = "rare"
    }),
    
    knightShield = Weapon:new(WeaponType.SHIELD, {
        name = "Knight's Shield",
        stats = {CON = 5, armor = 10},
        requiredLevel = 10,
        quality = "uncommon"
    }),
    
    -- Ranger weapons
    huntersBow = Weapon:new(WeaponType.BOW, {
        name = "Hunter's Bow",
        damageBonus = 4,
        stats = {DEX = 8, critChance = 3},
        requiredLevel = 10,
        quality = "uncommon"
    })
}

-- Example armor pieces
local ExampleArmor = {
    -- Cloth (Wizard, Sorcerer, Priest, Warlock)
    silkHelmet = Item:new({
        name = "Silk Helmet",
        slot = EquipmentSlot.HELMET,
        armorType = ArmorType.CLOTH,
        armor = 2,
        stats = {INT = 5, spellpower = 10},
        requiredLevel = 5
    }),
    
    robesOfPower = Item:new({
        name = "Robes of Arcane Power",
        slot = EquipmentSlot.CHEST,
        armorType = ArmorType.CLOTH,
        armor = 5,
        stats = {INT = 15, spellpower = 40},
        requiredLevel = 20,
        quality = "epic"
    }),
    
    -- Leather (Ranger, Druid, Rogue)
    leatherHelm = Item:new({
        name = "Leather Helm",
        slot = EquipmentSlot.HELMET,
        armorType = ArmorType.LEATHER,
        armor = 4,
        stats = {DEX = 3, AGI = 2},
        requiredLevel = 5
    }),
    
    trackersTunic = Item:new({
        name = "Tracker's Tunic",
        slot = EquipmentSlot.CHEST,
        armorType = ArmorType.LEATHER,
        armor = 10,
        stats = {DEX = 10, AGI = 8},
        requiredLevel = 15,
        quality = "rare"
    }),
    
    -- Mail (Priest, Fighter, Ranger)
    chainHelm = Item:new({
        name = "Chainmail Helm",
        slot = EquipmentSlot.HELMET,
        armorType = ArmorType.MAIL,
        armor = 8,
        stats = {CON = 5},
        requiredLevel = 10
    }),
    
    -- Plate (Fighter only typically)
    plateHelm = Item:new({
        name = "Plate Helmet",
        slot = EquipmentSlot.HELMET,
        armorType = ArmorType.PLATE,
        armor = 15,
        stats = {STR = 5, CON = 8},
        requiredLevel = 20,
        quality = "rare"
    }),
    
    titanPlate = Item:new({
        name = "Titan Plate Armor",
        slot = EquipmentSlot.CHEST,
        armorType = ArmorType.PLATE,
        armor = 30,
        stats = {STR = 15, CON = 20},
        requiredLevel = 30,
        quality = "epic"
    }),
    
    -- Shoulder
    mantle = Item:new({
        name = "Mantle of the Bear",
        slot = EquipmentSlot.SHOULDER,
        armorType = ArmorType.LEATHER,
        armor = 6,
        stats = {STR = 8, CON = 6},
        requiredLevel = 15
    }),
    
    -- Belt
    girdle = Item:new({
        name = "Girdle of Giant Strength",
        slot = EquipmentSlot.BELT,
        armorType = ArmorType.LEATHER,
        armor = 2,
        stats = {STR = 12},
        requiredLevel = 20,
        quality = "rare"
    }),
    
    -- Bracers
    bracers = Item:new({
        name = "Bracers of Defense",
        slot = EquipmentSlot.BRACER,
        armorType = ArmorType.CLOTH,
        armor = 3,
        stats = {armor = 5, DEX = 4},
        requiredLevel = 10
    }),
    
    -- Gloves
    gloves = Item:new({
        name = "Gloves of Dexterity",
        slot = EquipmentSlot.GLOVES,
        armorType = ArmorType.LEATHER,
        armor = 3,
        stats = {DEX = 10},
        requiredLevel = 15,
        quality = "uncommon"
    }),
    
    -- Legs
    leggings = Item:new({
        name = "Leggings of the Wild",
        slot = EquipmentSlot.LEGS,
        armorType = ArmorType.LEATHER,
        armor = 8,
        stats = {DEX = 8, AGI = 6, CON = 4},
        requiredLevel = 18
    }),
    
    -- Boots
    boots = Item:new({
        name = "Boots of Speed",
        slot = EquipmentSlot.BOOTS,
        armorType = ArmorType.LEATHER,
        armor = 4,
        stats = {DEX = 6, movementSpeed = 10},
        requiredLevel = 12
    })
}

-- Jewelry
local ExampleJewelry = {
    necklace1 = Item:new({
        name = "Amulet of Intelligence",
        slot = EquipmentSlot.NECKLACE,
        stats = {INT = 15, spellpower = 30},
        requiredLevel = 15,
        quality = "rare"
    }),
    
    necklace2 = Item:new({
        name = "Pendant of Strength",
        slot = EquipmentSlot.NECKLACE,
        stats = {STR = 12, critDamage = 20},
        requiredLevel = 15,
        quality = "rare"
    }),
    
    ring1 = Item:new({
        name = "Ring of Power",
        slot = EquipmentSlot.RING1,
        stats = {INT = 8, CHA = 6},
        requiredLevel = 10,
        quality = "uncommon"
    }),
    
    ring2 = Item:new({
        name = "Band of the Bear",
        slot = EquipmentSlot.RING1,  -- Can go in either ring slot
        stats = {STR = 10, CON = 8},
        requiredLevel = 12,
        quality = "uncommon"
    }),
    
    ring3 = Item:new({
        name = "Ring of Regeneration",
        slot = EquipmentSlot.RING1,
        stats = {healthRegen = 5, resourceRegen = 3},
        requiredLevel = 20,
        quality = "epic"
    })
}

function onLoad()
    log("Equipment System Plugin loaded!")
    log("Equipment slots: " .. countTable(EquipmentSlot))
    log("Example weapons: " .. countTable(ExampleWeapons))
    log("Example armor: " .. countTable(ExampleArmor))
    log("Example jewelry: " .. countTable(ExampleJewelry))
end

function canEquipWeapon(weapon, playerClass)
    if not playerClass then
        log("No class selected!")
        return false
    end
    
    -- Check if weapon type is allowed
    local weaponCategory = weapon.type
    local allowed = false
    for _, allowedType in ipairs(playerClass.allowedWeapons) do
        if weaponCategory == allowedType then
            allowed = true
            break
        end
    end
    
    if not allowed then
        log(playerClass.name .. " cannot use " .. weapon.name)
        return false
    end
    
    return true
end

function equipWeapon(weapon, slot, playerClass)
    if not canEquipWeapon(weapon, playerClass) then
        return false
    end
    
    -- Check if 2-handed weapon
    if weapon.hands == 2 then
        if EquippedItems.offHand then
            log("Cannot equip 2-handed weapon with item in off-hand")
            return false
        end
        EquippedItems.mainHand = weapon
        log("Equipped " .. weapon.name .. " (2H)")
    elseif slot == EquipmentSlot.MAIN_HAND then
        EquippedItems.mainHand = weapon
        log("Equipped " .. weapon.name .. " in main hand")
    elseif slot == EquipmentSlot.OFF_HAND then
        -- Check if 2-handed weapon in main hand
        if EquippedItems.mainHand and EquippedItems.mainHand.hands == 2 then
            log("Cannot equip off-hand with 2-handed weapon")
            return false
        end
        
        -- Check dual-wield or shield
        if weapon.block then
            if not playerClass.canUseShield then
                log(playerClass.name .. " cannot use shields")
                return false
            end
            EquippedItems.offHand = weapon
            log("Equipped " .. weapon.name .. " (shield)")
        else
            if not playerClass.canDualWield then
                log(playerClass.name .. " cannot dual-wield")
                return false
            end
            EquippedItems.offHand = weapon
            log("Equipped " .. weapon.name .. " in off-hand (dual-wield)")
        end
    end
    
    return true
end

function equipArmor(item, playerClass)
    if not playerClass then
        log("No class selected!")
        return false
    end
    
    -- Check if armor type is allowed
    local allowed = false
    for _, allowedType in ipairs(playerClass.allowedArmor) do
        if item.armorType == allowedType then
            allowed = true
            break
        end
    end
    
    if not allowed then
        log(playerClass.name .. " cannot wear " .. item.armorType .. " armor")
        return false
    end
    
    EquippedItems[item.slot] = item
    log("Equipped " .. item.name .. " in " .. item.slot)
    return true
end

function equipJewelry(item, slot)
    EquippedItems[slot] = item
    log("Equipped " .. item.name .. " in " .. slot)
    return true
end

function unequip(slot)
    if EquippedItems[slot] then
        log("Unequipped " .. EquippedItems[slot].name)
        EquippedItems[slot] = nil
    end
end

function getTotalStats()
    local totalStats = {
        STR = 0, DEX = 0, CON = 0,
        INT = 0, WIS = 0, CHA = 0,
        armor = 0, spellpower = 0
    }
    
    for slot, item in pairs(EquippedItems) do
        if item then
            -- Add armor value
            if item.armor then
                totalStats.armor = totalStats.armor + item.armor
            end
            
            -- Add all stats
            if item.stats then
                for stat, value in pairs(item.stats) do
                    if totalStats[stat] then
                        totalStats[stat] = totalStats[stat] + value
                    else
                        totalStats[stat] = value
                    end
                end
            end
            
            -- Add weapon damage
            if item.damage then
                if not totalStats.damage then
                    totalStats.damage = 0
                end
                totalStats.damage = totalStats.damage + item.damage
            end
        end
    end
    
    return totalStats
end

function displayEquipment()
    log("=== Equipment ===")
    log("Weapons:")
    log("  Main Hand: " .. (EquippedItems.mainHand and EquippedItems.mainHand.name or "Empty"))
    log("  Off Hand: " .. (EquippedItems.offHand and EquippedItems.offHand.name or "Empty"))
    log("")
    log("Armor:")
    log("  Helmet: " .. (EquippedItems.helmet and EquippedItems.helmet.name or "Empty"))
    log("  Shoulder: " .. (EquippedItems.shoulder and EquippedItems.shoulder.name or "Empty"))
    log("  Chest: " .. (EquippedItems.chest and EquippedItems.chest.name or "Empty"))
    log("  Belt: " .. (EquippedItems.belt and EquippedItems.belt.name or "Empty"))
    log("  Bracer: " .. (EquippedItems.bracer and EquippedItems.bracer.name or "Empty"))
    log("  Gloves: " .. (EquippedItems.gloves and EquippedItems.gloves.name or "Empty"))
    log("  Legs: " .. (EquippedItems.legs and EquippedItems.legs.name or "Empty"))
    log("  Boots: " .. (EquippedItems.boots and EquippedItems.boots.name or "Empty"))
    log("")
    log("Jewelry:")
    log("  Necklace: " .. (EquippedItems.necklace and EquippedItems.necklace.name or "Empty"))
    log("  Ring 1: " .. (EquippedItems.ring1 and EquippedItems.ring1.name or "Empty"))
    log("  Ring 2: " .. (EquippedItems.ring2 and EquippedItems.ring2.name or "Empty"))
    log("")
    log("=== Total Stats from Equipment ===")
    local stats = getTotalStats()
    for stat, value in pairs(stats) do
        if value > 0 then
            log("  " .. stat .. ": +" .. value)
        end
    end
end

function countTable(t)
    local count = 0
    for _ in pairs(t) do
        count = count + 1
    end
    return count
end
