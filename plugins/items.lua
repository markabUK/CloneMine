-- Items Plugin
-- Demonstrates adding items, tools, and materials

-- Item categories
local ItemCategory = {
    MATERIAL = 1,
    TOOL = 2,
    CONSUMABLE = 3,
    MISC = 4
}

-- Item registry
local items = {}
local nextItemId = 1000  -- Start custom items at ID 1000

-- Item class
local Item = {}
Item.__index = Item

function Item:new(properties)
    local item = setmetatable({}, Item)
    item.id = nextItemId
    nextItemId = nextItemId + 1
    
    item.name = properties.name or "Unknown Item"
    item.category = properties.category or ItemCategory.MISC
    item.stackSize = properties.stackSize or 64
    item.description = properties.description or ""
    item.durability = properties.durability or 0
    item.rarity = properties.rarity or "common"
    
    items[item.id] = item
    return item
end

-- Material items
local materials = {
    stick = Item:new({
        name = "Stick",
        category = ItemCategory.MATERIAL,
        stackSize = 64,
        description = "Used for crafting tools and weapons"
    }),
    
    coal = Item:new({
        name = "Coal",
        category = ItemCategory.MATERIAL,
        stackSize = 64,
        description = "Fuel source, used for smelting"
    }),
    
    iron_ingot = Item:new({
        name = "Iron Ingot",
        category = ItemCategory.MATERIAL,
        stackSize = 64,
        description = "Refined iron, used for crafting"
    }),
    
    gold_ingot = Item:new({
        name = "Gold Ingot",
        category = ItemCategory.MATERIAL,
        stackSize = 64,
        description = "Precious metal, used for crafting"
    }),
    
    diamond = Item:new({
        name = "Diamond",
        category = ItemCategory.MATERIAL,
        stackSize = 64,
        description = "Rare gem, used for powerful equipment",
        rarity = "rare"
    }),
    
    emerald = Item:new({
        name = "Emerald",
        category = ItemCategory.MATERIAL,
        stackSize = 64,
        description = "Precious gem, trading currency",
        rarity = "rare"
    }),
    
    leather = Item:new({
        name = "Leather",
        category = ItemCategory.MATERIAL,
        stackSize = 64,
        description = "Used for armor crafting"
    }),
    
    string = Item:new({
        name = "String",
        category = ItemCategory.MATERIAL,
        stackSize = 64,
        description = "Used for bows and fishing rods"
    })
}

-- Tool items
local tools = {
    wooden_pickaxe = Item:new({
        name = "Wooden Pickaxe",
        category = ItemCategory.TOOL,
        stackSize = 1,
        durability = 59,
        description = "Basic mining tool",
        miningSpeed = 2.0
    }),
    
    stone_pickaxe = Item:new({
        name = "Stone Pickaxe",
        category = ItemCategory.TOOL,
        stackSize = 1,
        durability = 131,
        description = "Improved mining tool",
        miningSpeed = 4.0
    }),
    
    iron_pickaxe = Item:new({
        name = "Iron Pickaxe",
        category = ItemCategory.TOOL,
        stackSize = 1,
        durability = 250,
        description = "Strong mining tool",
        miningSpeed = 6.0
    }),
    
    diamond_pickaxe = Item:new({
        name = "Diamond Pickaxe",
        category = ItemCategory.TOOL,
        stackSize = 1,
        durability = 1561,
        description = "Powerful mining tool",
        miningSpeed = 8.0,
        rarity = "rare"
    })
}

-- Consumable items
local consumables = {
    apple = Item:new({
        name = "Apple",
        category = ItemCategory.CONSUMABLE,
        stackSize = 64,
        description = "Restores 4 hunger points",
        healAmount = 4
    }),
    
    bread = Item:new({
        name = "Bread",
        category = ItemCategory.CONSUMABLE,
        stackSize = 64,
        description = "Restores 5 hunger points",
        healAmount = 5
    }),
    
    cooked_meat = Item:new({
        name = "Cooked Meat",
        category = ItemCategory.CONSUMABLE,
        stackSize = 64,
        description = "Restores 8 hunger points",
        healAmount = 8
    }),
    
    golden_apple = Item:new({
        name = "Golden Apple",
        category = ItemCategory.CONSUMABLE,
        stackSize = 64,
        description = "Restores 4 hunger + regeneration",
        healAmount = 4,
        effects = {"regeneration"},
        rarity = "rare"
    })
}

function onLoad()
    log("Items Plugin loaded!")
    
    local totalItems = 0
    for _ in pairs(items) do
        totalItems = totalItems + 1
    end
    
    log("Registered " .. totalItems .. " items:")
    log("  - Materials: " .. countCategory(ItemCategory.MATERIAL))
    log("  - Tools: " .. countCategory(ItemCategory.TOOL))
    log("  - Consumables: " .. countCategory(ItemCategory.CONSUMABLE))
end

function countCategory(category)
    local count = 0
    for _, item in pairs(items) do
        if item.category == category then
            count = count + 1
        end
    end
    return count
end

-- Get item by ID
function getItem(itemId)
    return items[itemId]
end

-- Get all items
function getAllItems()
    return items
end

-- Use consumable item
function useConsumable(itemId, player)
    local item = items[itemId]
    if item and item.category == ItemCategory.CONSUMABLE then
        log("Player consumed " .. item.name)
        if item.healAmount then
            -- Heal player (placeholder)
            log("  Healed " .. item.healAmount .. " hunger points")
        end
        if item.effects then
            for _, effect in ipairs(item.effects) do
                log("  Applied effect: " .. effect)
            end
        end
        return true
    end
    return false
end
