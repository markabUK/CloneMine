-- Custom Blocks Plugin
-- Demonstrates adding new block types to the game

-- Block type IDs (custom blocks start after vanilla ones)
local CUSTOM_BLOCKS = {
    OBSIDIAN = 10,
    GOLD_ORE = 11,
    DIAMOND_ORE = 12,
    EMERALD_ORE = 13,
    GLASS = 14,
    BRICK = 15,
    ICE = 16,
    LAVA = 17
}

-- Block definitions with properties
local blockDefinitions = {
    [CUSTOM_BLOCKS.OBSIDIAN] = {
        name = "Obsidian",
        hardness = 50.0,
        blastResistance = 1200.0,
        transparent = false,
        solid = true,
        luminance = 0
    },
    [CUSTOM_BLOCKS.GOLD_ORE] = {
        name = "Gold Ore",
        hardness = 3.0,
        blastResistance = 3.0,
        transparent = false,
        solid = true,
        luminance = 0,
        dropsItem = "gold_nugget"
    },
    [CUSTOM_BLOCKS.DIAMOND_ORE] = {
        name = "Diamond Ore",
        hardness = 3.0,
        blastResistance = 3.0,
        transparent = false,
        solid = true,
        luminance = 0,
        dropsItem = "diamond"
    },
    [CUSTOM_BLOCKS.EMERALD_ORE] = {
        name = "Emerald Ore",
        hardness = 3.0,
        blastResistance = 3.0,
        transparent = false,
        solid = true,
        luminance = 0,
        dropsItem = "emerald"
    },
    [CUSTOM_BLOCKS.GLASS] = {
        name = "Glass",
        hardness = 0.3,
        blastResistance = 0.3,
        transparent = true,
        solid = true,
        luminance = 0
    },
    [CUSTOM_BLOCKS.BRICK] = {
        name = "Brick",
        hardness = 2.0,
        blastResistance = 6.0,
        transparent = false,
        solid = true,
        luminance = 0
    },
    [CUSTOM_BLOCKS.ICE] = {
        name = "Ice",
        hardness = 0.5,
        blastResistance = 0.5,
        transparent = true,
        solid = true,
        luminance = 0,
        slippery = true
    },
    [CUSTOM_BLOCKS.LAVA] = {
        name = "Lava",
        hardness = 100.0,
        blastResistance = 100.0,
        transparent = false,
        solid = false,
        luminance = 15,
        damage = 4.0
    }
}

function onLoad()
    log("Custom Blocks Plugin loaded!")
    log("Added " .. getTableSize(blockDefinitions) .. " new block types")
    
    -- Register all custom blocks
    for blockId, def in pairs(blockDefinitions) do
        log("  - " .. def.name .. " (ID: " .. blockId .. ")")
    end
end

function onUpdate(deltaTime)
    -- Could add block behavior updates here
    -- For example: ice melting, lava spreading, etc.
end

-- Helper function to get block definition
function getBlockDefinition(blockId)
    return blockDefinitions[blockId]
end

-- Helper function to check if a block is custom
function isCustomBlock(blockId)
    return blockDefinitions[blockId] ~= nil
end

-- Utility function to get table size
function getTableSize(t)
    local count = 0
    for _ in pairs(t) do
        count = count + 1
    end
    return count
end

-- Example: Generate ore veins in world
function generateOreVein(x, y, z, oreType, size)
    local placed = 0
    for dx = -size, size do
        for dy = -size, size do
            for dz = -size, size do
                -- Spherical distribution
                local dist = math.sqrt(dx*dx + dy*dy + dz*dz)
                if dist <= size and math.random() < 0.5 then
                    setBlockType(x + dx, y + dy, z + dz, oreType)
                    placed = placed + 1
                end
            end
        end
    end
    return placed
end
