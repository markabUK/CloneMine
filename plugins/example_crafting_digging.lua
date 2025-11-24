-- Minecraft-Style Block Digging and Crafting System
-- Complete mining, digging, and crafting mechanics

return {
    -- ========================================
    -- BLOCK DIGGING SYSTEM
    -- ========================================
    digging = {
        -- Block hardness values (time to mine with correct tool)
        blockHardness = {
            -- Soft blocks (instant or very fast)
            GRASS = 0.6,
            DIRT = 0.5,
            SAND = 0.5,
            GRAVEL = 0.6,
            SNOW = 0.1,
            
            -- Wood blocks
            WOOD = 2.0,
            PLANKS = 2.0,
            LOG = 2.0,
            
            -- Stone blocks
            STONE = 1.5,
            COBBLESTONE = 2.0,
            SANDSTONE = 0.8,
            
            -- Ores
            COAL_ORE = 3.0,
            IRON_ORE = 3.0,
            GOLD_ORE = 3.0,
            DIAMOND_ORE = 3.0,
            EMERALD_ORE = 3.0,
            
            -- Hard blocks
            OBSIDIAN = 50.0,
            BEDROCK = -1,  -- Unbreakable
            
            -- Special blocks
            GLASS = 0.3,
            ICE = 0.5,
            BRICK = 2.0,
            
            -- Custom blocks (from plugins)
            LAVA_BLOCK = -1,  -- Unbreakable (liquid)
            ENCHANTED_STONE = 10.0
        },
        
        -- Tool effectiveness multipliers
        toolEffectiveness = {
            -- Pickaxe (best for stone/ore)
            PICKAXE = {
                STONE = 1.0,
                COBBLESTONE = 1.0,
                SANDSTONE = 1.0,
                COAL_ORE = 1.0,
                IRON_ORE = 1.0,
                GOLD_ORE = 1.0,
                DIAMOND_ORE = 1.0,
                EMERALD_ORE = 1.0,
                OBSIDIAN = 1.0,
                BRICK = 1.0,
                default = 0.2  -- Poor on non-stone
            },
            
            -- Axe (best for wood)
            AXE = {
                WOOD = 1.0,
                PLANKS = 1.0,
                LOG = 1.0,
                default = 0.2
            },
            
            -- Shovel (best for soft blocks)
            SHOVEL = {
                DIRT = 1.0,
                GRASS = 1.0,
                SAND = 1.0,
                GRAVEL = 1.0,
                SNOW = 1.0,
                default = 0.2
            },
            
            -- Sword (combat, poor for blocks)
            SWORD = {
                default = 0.1
            },
            
            -- Hand (no tool)
            HAND = {
                default = 0.05  -- Very slow
            }
        },
        
        -- Tool tier mining speeds
        toolTiers = {
            HAND = {speed = 1.0, canMine = {"DIRT", "GRASS", "SAND", "GRAVEL", "WOOD", "PLANKS"}},
            WOODEN = {speed = 2.0, canMine = {"STONE", "COAL_ORE"}},
            STONE = {speed = 4.0, canMine = {"IRON_ORE"}},
            IRON = {speed = 6.0, canMine = {"GOLD_ORE", "DIAMOND_ORE", "EMERALD_ORE"}},
            DIAMOND = {speed = 8.0, canMine = {"OBSIDIAN"}},
            NETHERITE = {speed = 9.0, canMine = {"ALL"}}  -- Can mine anything
        },
        
        -- Stat bonuses to mining
        statBonuses = {
            STR = 0.01,  -- +1% mining speed per STR
            DEX = 0.005  -- +0.5% mining speed per DEX
        },
        
        -- Calculate mining time
        calculateMiningTime = function(self, block, tool, toolTier, playerStats)
            local hardness = self.blockHardness[block] or 1.0
            
            if hardness == -1 then
                return -1  -- Unbreakable
            end
            
            -- Check if tool can mine this block
            local tierInfo = self.toolTiers[toolTier]
            if tierInfo and tierInfo.canMine then
                if tierInfo.canMine[1] ~= "ALL" then
                    local canMine = false
                    for _, allowedBlock in ipairs(tierInfo.canMine) do
                        if allowedBlock == block then
                            canMine = true
                            break
                        end
                    end
                    if not canMine then
                        return -1  -- Wrong tool tier
                    end
                end
            end
            
            -- Base mining time
            local time = hardness
            
            -- Apply tool effectiveness
            local effectiveness = 1.0
            if tool and self.toolEffectiveness[tool] then
                effectiveness = self.toolEffectiveness[tool][block] or self.toolEffectiveness[tool].default
            else
                effectiveness = self.toolEffectiveness.HAND.default
            end
            
            time = time / effectiveness
            
            -- Apply tool tier speed
            if tierInfo then
                time = time / tierInfo.speed
            end
            
            -- Apply stat bonuses
            if playerStats then
                local statBonus = 1.0
                statBonus = statBonus + (playerStats.STR or 0) * self.statBonuses.STR
                statBonus = statBonus + (playerStats.DEX or 0) * self.statBonuses.DEX
                time = time / statBonus
            end
            
            return time
        end,
        
        -- Block drops
        blockDrops = {
            STONE = {{item = "COBBLESTONE", count = 1, chance = 1.0}},
            COBBLESTONE = {{item = "COBBLESTONE", count = 1, chance = 1.0}},
            DIRT = {{item = "DIRT", count = 1, chance = 1.0}},
            GRASS = {{item = "DIRT", count = 1, chance = 1.0}},
            WOOD = {{item = "WOOD", count = 1, chance = 1.0}},
            LOG = {{item = "LOG", count = 1, chance = 1.0}},
            
            -- Ores drop items
            COAL_ORE = {{item = "COAL", count = 1, chance = 1.0}},
            IRON_ORE = {{item = "IRON_ORE", count = 1, chance = 1.0}},  -- Needs smelting
            GOLD_ORE = {{item = "GOLD_ORE", count = 1, chance = 1.0}},
            DIAMOND_ORE = {{item = "DIAMOND", count = 1, chance = 1.0}},
            EMERALD_ORE = {{item = "EMERALD", count = 1, chance = 1.0}},
            
            -- Special drops
            GLASS = {},  -- Breaks without drop unless silk touch
            ICE = {{item = "WATER", count = 1, chance = 1.0}},
            
            OBSIDIAN = {{item = "OBSIDIAN", count = 1, chance = 1.0}}
        },
        
        -- Fortune enchantment (increases drops)
        fortuneMultiplier = {
            [0] = 1,  -- No fortune
            [1] = {min = 1, max = 2},
            [2] = {min = 1, max = 3},
            [3] = {min = 1, max = 4}
        }
    },
    
    -- ========================================
    -- CRAFTING SYSTEM
    -- ========================================
    crafting = {
        -- Crafting table recipes (3x3 grid)
        recipes = {
            -- Tools
            {
                name = "Wooden Pickaxe",
                pattern = {
                    {"PLANKS", "PLANKS", "PLANKS"},
                    {"", "STICK", ""},
                    {"", "STICK", ""}
                },
                result = {item = "WOODEN_PICKAXE", count = 1}
            },
            {
                name = "Stone Pickaxe",
                pattern = {
                    {"COBBLESTONE", "COBBLESTONE", "COBBLESTONE"},
                    {"", "STICK", ""},
                    {"", "STICK", ""}
                },
                result = {item = "STONE_PICKAXE", count = 1}
            },
            {
                name = "Iron Pickaxe",
                pattern = {
                    {"IRON_INGOT", "IRON_INGOT", "IRON_INGOT"},
                    {"", "STICK", ""},
                    {"", "STICK", ""}
                },
                result = {item = "IRON_PICKAXE", count = 1}
            },
            {
                name = "Diamond Pickaxe",
                pattern = {
                    {"DIAMOND", "DIAMOND", "DIAMOND"},
                    {"", "STICK", ""},
                    {"", "STICK", ""}
                },
                result = {item = "DIAMOND_PICKAXE", count = 1}
            },
            
            -- Axes
            {
                name = "Iron Axe",
                pattern = {
                    {"IRON_INGOT", "IRON_INGOT", ""},
                    {"IRON_INGOT", "STICK", ""},
                    {"", "STICK", ""}
                },
                result = {item = "IRON_AXE", count = 1}
            },
            
            -- Swords
            {
                name = "Iron Sword",
                pattern = {
                    {"", "IRON_INGOT", ""},
                    {"", "IRON_INGOT", ""},
                    {"", "STICK", ""}
                },
                result = {item = "IRON_SWORD", count = 1}
            },
            {
                name = "Diamond Sword",
                pattern = {
                    {"", "DIAMOND", ""},
                    {"", "DIAMOND", ""},
                    {"", "STICK", ""}
                },
                result = {item = "DIAMOND_SWORD", count = 1}
            },
            
            -- Armor
            {
                name = "Iron Helmet",
                pattern = {
                    {"IRON_INGOT", "IRON_INGOT", "IRON_INGOT"},
                    {"IRON_INGOT", "", "IRON_INGOT"},
                    {"", "", ""}
                },
                result = {item = "IRON_HELMET", count = 1}
            },
            {
                name = "Iron Chestplate",
                pattern = {
                    {"IRON_INGOT", "", "IRON_INGOT"},
                    {"IRON_INGOT", "IRON_INGOT", "IRON_INGOT"},
                    {"IRON_INGOT", "IRON_INGOT", "IRON_INGOT"}
                },
                result = {item = "IRON_CHESTPLATE", count = 1}
            },
            {
                name = "Iron Leggings",
                pattern = {
                    {"IRON_INGOT", "IRON_INGOT", "IRON_INGOT"},
                    {"IRON_INGOT", "", "IRON_INGOT"},
                    {"IRON_INGOT", "", "IRON_INGOT"}
                },
                result = {item = "IRON_LEGGINGS", count = 1}
            },
            {
                name = "Iron Boots",
                pattern = {
                    {"", "", ""},
                    {"IRON_INGOT", "", "IRON_INGOT"},
                    {"IRON_INGOT", "", "IRON_INGOT"}
                },
                result = {item = "IRON_BOOTS", count = 1}
            },
            
            -- Building blocks
            {
                name = "Planks from Log",
                pattern = {
                    {"LOG", "", ""},
                    {"", "", ""},
                    {"", "", ""}
                },
                result = {item = "PLANKS", count = 4}
            },
            {
                name = "Sticks",
                pattern = {
                    {"PLANKS", "", ""},
                    {"PLANKS", "", ""},
                    {"", "", ""}
                },
                result = {item = "STICK", count = 4}
            },
            {
                name = "Crafting Table",
                pattern = {
                    {"PLANKS", "PLANKS", ""},
                    {"PLANKS", "PLANKS", ""},
                    {"", "", ""}
                },
                result = {item = "CRAFTING_TABLE", count = 1}
            },
            {
                name = "Chest",
                pattern = {
                    {"PLANKS", "PLANKS", "PLANKS"},
                    {"PLANKS", "", "PLANKS"},
                    {"PLANKS", "PLANKS", "PLANKS"}
                },
                result = {item = "CHEST", count = 1}
            },
            {
                name = "Furnace",
                pattern = {
                    {"COBBLESTONE", "COBBLESTONE", "COBBLESTONE"},
                    {"COBBLESTONE", "", "COBBLESTONE"},
                    {"COBBLESTONE", "COBBLESTONE", "COBBLESTONE"}
                },
                result = {item = "FURNACE", count = 1}
            },
            
            -- Advanced items
            {
                name = "Enchanting Table",
                pattern = {
                    {"", "BOOK", ""},
                    {"DIAMOND", "OBSIDIAN", "DIAMOND"},
                    {"OBSIDIAN", "OBSIDIAN", "OBSIDIAN"}
                },
                result = {item = "ENCHANTING_TABLE", count = 1}
            },
            {
                name = "Anvil",
                pattern = {
                    {"IRON_BLOCK", "IRON_BLOCK", "IRON_BLOCK"},
                    {"", "IRON_INGOT", ""},
                    {"IRON_INGOT", "IRON_INGOT", "IRON_INGOT"}
                },
                result = {item = "ANVIL", count = 1}
            }
        },
        
        -- Smelting recipes (furnace)
        smelting = {
            {input = "IRON_ORE", fuel = "COAL", result = "IRON_INGOT", time = 10},
            {input = "GOLD_ORE", fuel = "COAL", result = "GOLD_INGOT", time = 10},
            {input = "SAND", fuel = "COAL", result = "GLASS", time = 10},
            {input = "COBBLESTONE", fuel = "COAL", result = "STONE", time = 10},
            {input = "RAW_MEAT", fuel = "COAL", result = "COOKED_MEAT", time = 5},
            {input = "LOG", fuel = "COAL", result = "CHARCOAL", time = 10}
        },
        
        -- Shapeless recipes (order doesn't matter)
        shapeless = {
            {
                name = "Flint and Steel",
                ingredients = {"IRON_INGOT", "FLINT"},
                result = {item = "FLINT_AND_STEEL", count = 1}
            },
            {
                name = "Fire Charge",
                ingredients = {"BLAZE_POWDER", "COAL", "GUNPOWDER"},
                result = {item = "FIRE_CHARGE", count = 3}
            }
        },
        
        -- Check if recipe matches pattern
        matchRecipe = function(self, grid)
            for _, recipe in ipairs(self.recipes) do
                local matches = true
                for i = 1, 3 do
                    for j = 1, 3 do
                        local expected = recipe.pattern[i][j]
                        local actual = grid[i][j] or ""
                        if expected ~= actual then
                            matches = false
                            break
                        end
                    end
                    if not matches then break end
                end
                
                if matches then
                    return recipe.result
                end
            end
            return nil
        end
    },
    
    -- ========================================
    -- CUSTOM BLOCK LOGIC (Plugin Extension)
    -- ========================================
    customBlockLogic = {
        -- Example: Pressure plate
        PRESSURE_PLATE = {
            onStep = function(player, world, x, y, z)
                -- Activate redstone signal
                world.setPower(x, y, z, 15)
                -- Trigger connected devices
                world.triggerRedstone(x, y, z)
            end,
            onLeave = function(player, world, x, y, z)
                world.setPower(x, y, z, 0)
            end
        },
        
        -- Example: Door
        DOOR = {
            onInteract = function(player, world, x, y, z)
                local state = world.getBlockState(x, y, z)
                if state.open then
                    world.setBlockState(x, y, z, {open = false})
                    world.playSound("door_close", x, y, z)
                else
                    world.setBlockState(x, y, z, {open = true})
                    world.playSound("door_open", x, y, z)
                end
            end
        },
        
        -- Example: TNT
        TNT = {
            onIgnite = function(player, world, x, y, z)
                world.removeBlock(x, y, z)
                world.spawnEntity("TNT_PRIMED", x, y, z, {fuse = 80})  -- 4 seconds
            end,
            onExplode = function(world, x, y, z)
                world.createExplosion(x, y, z, 4, true)  -- Radius 4, destroys blocks
            end
        },
        
        -- Example: Furnace
        FURNACE = {
            onInteract = function(player, world, x, y, z)
                world.openGUI(player, "FURNACE", x, y, z)
            end,
            onUpdate = function(world, x, y, z)
                local furnace = world.getTileEntity(x, y, z)
                if furnace.isSmelting then
                    furnace.progress = furnace.progress + 1
                    if furnace.progress >= furnace.smeltTime then
                        -- Complete smelting
                        furnace.outputSlot = furnace.recipe.result
                        furnace.inputSlot.count = furnace.inputSlot.count - 1
                        furnace.fuelSlot.count = furnace.fuelSlot.count - 1
                        furnace.progress = 0
                    end
                end
            end
        },
        
        -- Example: Enchanting Table
        ENCHANTING_TABLE = {
            onInteract = function(player, world, x, y, z)
                local bookshelfCount = world.countNearbyBlocks(x, y, z, "BOOKSHELF", 2)
                local maxLevel = math.min(bookshelfCount, 30)
                world.openGUI(player, "ENCHANTING", x, y, z, {maxLevel = maxLevel})
            end
        }
    }
}
