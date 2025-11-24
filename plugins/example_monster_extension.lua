-- Example Plugin: Monster Extension
-- This demonstrates how to add new monsters and bosses to the game

-- Add custom regular monsters
function addCustomMonsters()
    local monsters = {
        {
            id = "FROST_GIANT",
            name = "Frost Giant",
            level = 15,
            difficulty = "Elite",
            health = 5000,
            damage = 80,
            armor = 40,
            resistances = {
                frost = 0.75,  -- 75% frost resistance
                fire = -0.50   -- Takes 50% more fire damage
            },
            abilities = {
                "Ice Smash",
                "Frozen Ground",
                "Glacial Armor"
            },
            loot = {
                {item = "FROST_GIANT_HEART", chance = 0.15},
                {item = "ICE_SHARD", chance = 0.40},
                {item = "BLUE_DIAMOND", chance = 0.05}
            },
            gold = {min = 100, max = 200},
            xp = 800,
            spawnLocations = {"MOUNTAINS", "UNDERGROUND"},
            spawnTime = {"DAY", "NIGHT"},
            description = "A massive giant from the frozen peaks, wielding the power of ice and frost"
        },
        {
            id = "SHADOW_STALKER",
            name = "Shadow Stalker",
            level = 12,
            difficulty = "Hard",
            health = 2000,
            damage = 100,  -- High burst damage
            armor = 15,    -- Low armor
            abilities = {
                "Shadow Strike",  -- Teleport behind target and attack
                "Vanish",         -- Become invisible for 5 seconds
                "Backstab"        -- Extra damage from behind
            },
            loot = {
                {item = "SHADOW_ESSENCE", chance = 0.25},
                {item = "DARK_DAGGER", chance = 0.10}
            },
            gold = {min = 50, max = 100},
            xp = 400,
            spawnLocations = {"FOREST", "UNDERGROUND"},
            spawnTime = {"NIGHT", "DUSK"},
            description = "A deadly assassin that strikes from the shadows when you least expect it"
        },
        {
            id = "CORRUPTED_TREANT",
            name = "Corrupted Treant",
            level = 10,
            difficulty = "Normal",
            health = 3000,
            damage = 50,
            armor = 50,  -- High armor
            immunities = {"NATURE", "POISON"},
            weaknesses = {"FIRE"},
            abilities = {
                "Root Grasp",      -- Root enemies in place
                "Bark Skin",       -- Increase armor by 50%
                "Nature's Wrath"   -- Summon smaller treants
            },
            loot = {
                {item = "ANCIENT_WOOD", chance = 0.50},
                {item = "NATURE_ESSENCE", chance = 0.30},
                {item = "TREANT_SEED", chance = 0.10}
            },
            gold = {min = 30, max = 80},
            xp = 300,
            spawnLocations = {"FOREST", "SWAMP"},
            spawnTime = {"DAY", "DAWN", "DUSK"},
            description = "Once a guardian of the forest, now twisted by dark magic"
        }
    }
    
    for _, monster in ipairs(monsters) do
        log("Registered custom monster: " .. monster.name .. " (Level " .. monster.level .. ")")
    end
    
    return monsters
end

-- Add custom boss encounters
function addCustomBosses()
    local bosses = {
        {
            id = "DRAGON_QUEEN",
            name = "Tiamat, Dragon Queen",
            level = 20,
            difficulty = "Raid Boss",
            health = 100000,
            damage = 200,
            armor = 100,
            phases = {
                {
                    threshold = 100,  -- Phase 1: 100% - 75% HP
                    abilities = {
                        "Multi-Head Breath",  -- Breaths from all 5 heads
                        "Tail Sweep",
                        "Wing Buffet"
                    },
                    description = "Tiamat attacks with all five draconic heads simultaneously"
                },
                {
                    threshold = 75,  -- Phase 2: 75% - 50% HP
                    abilities = {
                        "Elemental Fury",      -- Cycles through fire, frost, poison, lightning, acid
                        "Summon Dragonkin",    -- Summons 3 elite dragonkin
                        "Dragon Fear"          -- AOE fear
                    },
                    description = "Tiamat channels the power of all five chromatic dragon types"
                },
                {
                    threshold = 50,  -- Phase 3: 50% - 25% HP
                    abilities = {
                        "Cataclysm",           -- Massive AOE damage
                        "Chromatic Barrier",   -- Immune to one element at a time
                        "Regeneration"         -- Heals 1% HP per second
                    },
                    description = "Tiamat's rage intensifies as she nears defeat"
                },
                {
                    threshold = 25,  -- Phase 4: 25% - 0% HP
                    abilities = {
                        "Apocalypse Breath",   -- Ultimate attack that can wipe the raid
                        "Summon Dragon Gods",  -- Summons 5 powerful dragons
                        "Divine Shield",       -- Becomes invulnerable periodically
                        "Enrage"               -- Damage increases over time
                    },
                    description = "Tiamat unleashes her full divine power in a desperate final stand"
                }
            },
            loot = {
                {item = "TIAMAT_SCALE", chance = 1.0},
                {item = "LEGENDARY_DRAGON_WEAPON", chance = 0.30},
                {item = "LEGENDARY_DRAGON_ARMOR", chance = 0.30},
                {item = "DRAGON_QUEEN_CROWN", chance = 0.15},
                {item = "CHROMATIC_ORB", chance = 0.50}
            },
            gold = {min = 5000, max = 10000},
            xp = 50000,
            spawnLocation = "VOLCANIC",
            requiresGroup = true,
            minPlayers = 10,
            maxPlayers = 25,
            enrageTimer = 600,  -- 10 minute enrage timer
            description = "The five-headed queen of dragons, embodiment of chromatic destruction and chaos"
        },
        {
            id = "VOID_LORD",
            name = "Xal'atath, Void Lord",
            level = 20,
            difficulty = "Raid Boss",
            health = 80000,
            damage = 150,
            armor = 75,
            phases = {
                {
                    threshold = 100,
                    abilities = {
                        "Void Bolt",
                        "Shadow Crash",
                        "Mind Flay"
                    },
                    description = "The Void Lord assaults the raid with shadow magic"
                },
                {
                    threshold = 66,
                    abilities = {
                        "Void Rift",          -- Creates portals that spawn adds
                        "Mass Mind Control",  -- Controls 3 raid members
                        "Insanity"            -- Reduces healing by 50%
                    },
                    description = "Reality tears as void energy floods the battlefield"
                },
                {
                    threshold = 33,
                    abilities = {
                        "Void Form",          -- Transforms into pure void
                        "Annihilation",       -- One-shot mechanic that must be interrupted
                        "Eternal Darkness",   -- Room fills with void zones
                        "Spawn of the Void"   -- Summons powerful void creatures
                    },
                    description = "The Void Lord abandons physical form, becoming pure entropy"
                }
            },
            loot = {
                {item = "VOID_ESSENCE", chance = 1.0},
                {item = "LEGENDARY_VOID_WEAPON", chance = 0.25},
                {item = "VOID_LORD_ROBE", chance = 0.20},
                {item = "CROWN_OF_MADNESS", chance = 0.10},
                {item = "SHARD_OF_XALATATH", chance = 0.05}
            },
            gold = {min = 3000, max = 7000},
            xp = 40000,
            spawnLocation = "UNDERGROUND",
            requiresGroup = true,
            minPlayers = 8,
            maxPlayers = 20,
            enrageTimer = 480,  -- 8 minute enrage timer
            description = "An ancient entity from the void between worlds, seeking to consume all reality"
        }
    }
    
    for _, boss in ipairs(bosses) do
        log("Registered custom boss: " .. boss.name .. " (Level " .. boss.level .. ", " .. boss.difficulty .. ")")
    end
    
    return bosses
end

-- Add custom world events
function addCustomWorldEvents()
    local events = {
        {
            id = "DEMON_INVASION",
            name = "Demonic Invasion",
            description = "Demons are invading from a rift in the sky!",
            duration = 1800,  -- 30 minutes
            waves = {
                {time = 0, monsters = {"LESSER_DEMON", "IMP"}, count = 10},
                {time = 300, monsters = {"DEMON_WARRIOR", "SUCCUBUS"}, count = 8},
                {time = 600, monsters = {"PIT_LORD"}, count = 3},
                {time = 900, monsters = {"DEMON_LORD"}, count = 1, isBoss = true}
            },
            rewards = {
                reputation = 500,
                currency = "DEMON_CURRENCY",
                loot = {"DEMONIC_ESSENCE", "CURSED_WEAPON"}
            },
            spawnLocations = {"PLAINS", "FOREST"},
            triggerCondition = "RANDOM",  -- or "QUEST", "TIME_BASED"
            rarity = "Rare"
        }
    }
    
    for _, event in ipairs(events) do
        log("Registered custom world event: " .. event.name)
    end
    
    return events
end

-- Initialize the extension
function onLoad()
    log("Loading Monster Extension Example Plugin")
    
    -- Register custom content
    addCustomMonsters()
    addCustomBosses()
    addCustomWorldEvents()
    
    log("Monster Extension Example Plugin loaded successfully!")
end

-- Call onLoad when plugin is loaded
onLoad()
