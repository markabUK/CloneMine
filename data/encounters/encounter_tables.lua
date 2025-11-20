-- Random Encounter System
-- Location-based and time-sensitive monster spawning

local EncounterTables = {
    version = "1.0.0"
}

-- Rarity chances (must sum to 100)
EncounterTables.rarityChances = {
    COMMON = 60,
    UNCOMMON = 25,
    RARE = 12,
    EPIC = 3
}

-- Time of day modifiers
EncounterTables.timeModifiers = {
    DAY = {
        undeadChance = 0.5,  -- 50% normal chance
        beastChance = 1.5,   -- 150% normal chance
        description = "Daytime (6:00-18:00)"
    },
    NIGHT = {
        undeadChance = 2.0,  -- 200% chance (undead more common)
        beastChance = 0.7,   -- 70% chance
        demonChance = 1.5,   -- 150% chance
        description = "Nighttime (18:00-6:00)"
    },
    DAWN = {
        undeadChance = 1.2,
        beastChance = 1.3,
        description = "Dawn (5:00-7:00)"
    },
    DUSK = {
        undeadChance = 1.3,
        beastChance = 1.2,
        demonChance = 1.1,
        description = "Dusk (17:00-19:00)"
    }
}

-- Encounter tables by location
EncounterTables.locations = {
    FOREST = {
        name = "Forest",
        description = "Dense woodland area",
        encounters = {
            -- Common (60% chance)
            COMMON = {
                {monster = "WOLF", weight = 40, minLevel = 1, maxLevel = 20},
                {monster = "BEAR", weight = 30, minLevel = 5, maxLevel = 20},
                {monster = "BANDIT", weight = 30, minLevel = 3, maxLevel = 18}
            },
            -- Uncommon (25% chance)
            UNCOMMON = {
                {monster = "DIRE_WOLF", weight = 40, minLevel = 8, maxLevel = 20},
                {monster = "TREANT", weight = 30, minLevel = 10, maxLevel = 20},
                {monster = "ORC_SCOUT", weight = 30, minLevel = 6, maxLevel = 16}
            },
            -- Rare (12% chance)
            RARE = {
                {monster = "FOREST_TROLL", weight = 50, minLevel = 12, maxLevel = 20},
                {monster = "WEREWOLF", weight = 50, minLevel = 14, maxLevel = 20}
            },
            -- Epic (3% chance)
            EPIC = {
                {monster = "ANCIENT_TREANT", weight = 60, minLevel = 18, maxLevel = 20},
                {monster = "FOREST_DRAKE", weight = 40, minLevel = 17, maxLevel = 20}
            }
        },
        tags = {"NATURE", "BEAST", "HUMANOID"}
    },
    
    PLAINS = {
        name = "Plains",
        description = "Open grasslands",
        encounters = {
            COMMON = {
                {monster = "GOBLIN", weight = 50, minLevel = 1, maxLevel = 15},
                {monster = "WILD_BOAR", weight = 30, minLevel = 2, maxLevel = 12},
                {monster = "BANDIT", weight = 20, minLevel = 3, maxLevel = 18}
            },
            UNCOMMON = {
                {monster = "HOBGOBLIN", weight = 40, minLevel = 6, maxLevel = 18},
                {monster = "CENTAUR", weight = 35, minLevel = 8, maxLevel = 18},
                {monster = "ORC_WARRIOR", weight = 25, minLevel = 7, maxLevel = 17}
            },
            RARE = {
                {monster = "OGRE", weight = 50, minLevel = 12, maxLevel = 20},
                {monster = "GRIFFON", weight = 50, minLevel = 14, maxLevel = 20}
            },
            EPIC = {
                {monster = "PLAINS_GIANT", weight = 70, minLevel = 18, maxLevel = 20},
                {monster = "WYVERN", weight = 30, minLevel = 16, maxLevel = 20}
            }
        },
        tags = {"HUMANOID", "BEAST"}
    },
    
    MOUNTAINS = {
        name = "Mountains",
        description = "Rocky highlands",
        encounters = {
            COMMON = {
                {monster = "GOBLIN", weight = 30, minLevel = 3, maxLevel = 15},
                {monster = "MOUNTAIN_GOAT", weight = 20, minLevel = 1, maxLevel = 10},
                {monster = "EAGLE", weight = 50, minLevel = 4, maxLevel = 14}
            },
            UNCOMMON = {
                {monster = "ROCK_ELEMENTAL", weight = 40, minLevel = 8, maxLevel = 20},
                {monster = "TROLL", weight = 35, minLevel = 10, maxLevel = 20},
                {monster = "WYVERN", weight = 25, minLevel = 12, maxLevel = 20}
            },
            RARE = {
                {monster = "FROST_GIANT", weight = 50, minLevel = 15, maxLevel = 20},
                {monster = "YOUNG_DRAGON", weight = 50, minLevel = 14, maxLevel = 20}
            },
            EPIC = {
                {monster = "MOUNTAIN_GIANT", weight = 50, minLevel = 19, maxLevel = 20},
                {monster = "ADULT_DRAGON", weight = 50, minLevel = 20, maxLevel = 20}
            }
        },
        tags = {"ELEMENTAL", "GIANT", "DRAGON"}
    },
    
    SWAMP = {
        name = "Swamp",
        description = "Murky wetlands",
        encounters = {
            COMMON = {
                {monster = "SKELETON", weight = 35, minLevel = 2, maxLevel = 15, tag = "UNDEAD"},
                {monster = "ZOMBIE", weight = 30, minLevel = 3, maxLevel = 16, tag = "UNDEAD"},
                {monster = "GIANT_SPIDER", weight = 35, minLevel = 4, maxLevel = 14}
            },
            UNCOMMON = {
                {monster = "GHOUL", weight = 40, minLevel = 8, maxLevel = 18, tag = "UNDEAD"},
                {monster = "LIZARDMAN", weight = 35, minLevel = 7, maxLevel = 17},
                {monster = "BOG_BEAST", weight = 25, minLevel = 9, maxLevel = 18}
            },
            RARE = {
                {monster = "VAMPIRE", weight = 45, minLevel = 14, maxLevel = 20, tag = "UNDEAD"},
                {monster = "SWAMP_TROLL", weight = 55, minLevel = 13, maxLevel = 20}
            },
            EPIC = {
                {monster = "LICH", weight = 60, minLevel = 19, maxLevel = 20, tag = "UNDEAD"},
                {monster = "HYDRA", weight = 40, minLevel = 18, maxLevel = 20}
            }
        },
        tags = {"UNDEAD", "BEAST", "REPTILE"}
    },
    
    DESERT = {
        name = "Desert",
        description = "Arid wastelands",
        encounters = {
            COMMON = {
                {monster = "GIANT_SCORPION", weight = 45, minLevel = 3, maxLevel = 15},
                {monster = "SAND_SNAKE", weight = 30, minLevel = 2, maxLevel = 12},
                {monster = "NOMAD_BANDIT", weight = 25, minLevel = 4, maxLevel = 16}
            },
            UNCOMMON = {
                {monster = "SAND_WURM", weight = 40, minLevel = 8, maxLevel = 18},
                {monster = "MUMMY", weight = 35, minLevel = 10, maxLevel = 18, tag = "UNDEAD"},
                {monster = "DJINN", weight = 25, minLevel = 12, maxLevel = 19}
            },
            RARE = {
                {monster = "SANDSTORM_ELEMENTAL", weight = 50, minLevel = 14, maxLevel = 20},
                {monster = "SPHINX", weight = 50, minLevel = 16, maxLevel = 20}
            },
            EPIC = {
                {monster = "ANCIENT_WURM", weight = 60, minLevel = 19, maxLevel = 20},
                {monster = "EFREETI_LORD", weight = 40, minLevel = 20, maxLevel = 20}
            }
        },
        tags = {"BEAST", "ELEMENTAL", "UNDEAD"}
    },
    
    UNDERGROUND = {
        name = "Underground",
        description = "Dark caverns and tunnels",
        encounters = {
            COMMON = {
                {monster = "GIANT_RAT", weight = 35, minLevel = 1, maxLevel = 10},
                {monster = "CAVE_SPIDER", weight = 35, minLevel = 2, maxLevel = 12},
                {monster = "SKELETON", weight = 30, minLevel = 3, maxLevel = 15, tag = "UNDEAD"}
            },
            UNCOMMON = {
                {monster = "CAVE_TROLL", weight = 40, minLevel = 8, maxLevel = 18},
                {monster = "DARK_ELF", weight = 30, minLevel = 9, maxLevel = 18},
                {monster = "RUST_MONSTER", weight = 30, minLevel = 7, maxLevel = 16}
            },
            RARE = {
                {monster = "BEHOLDER", weight = 45, minLevel = 14, maxLevel = 20},
                {monster = "MIND_FLAYER", weight = 55, minLevel = 15, maxLevel = 20}
            },
            EPIC = {
                {monster = "ANCIENT_BEHOLDER", weight = 50, minLevel = 19, maxLevel = 20},
                {monster = "ELDER_BRAIN", weight = 50, minLevel = 20, maxLevel = 20}
            }
        },
        tags = {"BEAST", "ABERRATION", "UNDEAD"}
    },
    
    VOLCANIC = {
        name = "Volcanic",
        description = "Molten lava fields",
        encounters = {
            COMMON = {
                {monster = "FIRE_IMP", weight = 40, minLevel = 4, maxLevel = 15},
                {monster = "LAVA_BEETLE", weight = 35, minLevel = 3, maxLevel = 14},
                {monster = "ASH_ZOMBIE", weight = 25, minLevel = 5, maxLevel = 16, tag = "UNDEAD"}
            },
            UNCOMMON = {
                {monster = "FIRE_ELEMENTAL", weight = 40, minLevel = 10, maxLevel = 20},
                {monster = "MAGMA_GOLEM", weight = 35, minLevel = 11, maxLevel = 19},
                {monster = "HELLHOUND", weight = 25, minLevel = 9, maxLevel = 18}
            },
            RARE = {
                {monster = "BALROG", weight = 50, minLevel = 16, maxLevel = 20},
                {monster = "PHOENIX", weight = 50, minLevel = 17, maxLevel = 20}
            },
            EPIC = {
                {monster = "FIRE_DRAGON", weight = 60, minLevel = 20, maxLevel = 20},
                {monster = "DEMON_LORD", weight = 40, minLevel = 19, maxLevel = 20}
            }
        },
        tags = {"ELEMENTAL", "DEMON", "FIRE"}
    }
}

-- Roll for encounter rarity
function EncounterTables:rollRarity()
    local roll = math.random(1, 100)
    local cumulative = 0
    
    for rarity, chance in pairs(self.rarityChances) do
        cumulative = cumulative + chance
        if roll <= cumulative then
            return rarity
        end
    end
    
    return "COMMON"
end

-- Get time of day from hour (0-23)
function EncounterTables:getTimeOfDay(hour)
    if hour >= 5 and hour < 7 then
        return "DAWN"
    elseif hour >= 7 and hour < 17 then
        return "DAY"
    elseif hour >= 17 and hour < 19 then
        return "DUSK"
    else
        return "NIGHT"
    end
end

-- Select a random encounter from a weighted list
function EncounterTables:selectFromWeighted(encounters, playerLevel)
    -- Filter by level range
    local validEncounters = {}
    for _, enc in ipairs(encounters) do
        if playerLevel >= enc.minLevel and playerLevel <= enc.maxLevel then
            table.insert(validEncounters, enc)
        end
    end
    
    if #validEncounters == 0 then
        return nil
    end
    
    -- Calculate total weight
    local totalWeight = 0
    for _, enc in ipairs(validEncounters) do
        totalWeight = totalWeight + enc.weight
    end
    
    -- Roll for encounter
    local roll = math.random(1, totalWeight)
    local cumulative = 0
    
    for _, enc in ipairs(validEncounters) do
        cumulative = cumulative + enc.weight
        if roll <= cumulative then
            return enc.monster
        end
    end
    
    return validEncounters[1].monster
end

-- Roll for a random encounter
function EncounterTables:rollEncounter(location, timeOfDay, playerLevel)
    local locationData = self.locations[location:upper()]
    if not locationData then
        log("ERROR: Invalid location: " .. location)
        return nil
    end
    
    -- Get time modifiers
    local timeData = self.timeModifiers[timeOfDay:upper()] or self.timeModifiers.DAY
    
    -- Roll for rarity
    local rarity = self:rollRarity()
    
    -- Get encounters for this rarity
    local encounters = locationData.encounters[rarity]
    if not encounters or #encounters == 0 then
        -- Fall back to common
        encounters = locationData.encounters.COMMON
    end
    
    -- Apply time-of-day filtering
    local filteredEncounters = {}
    for _, enc in ipairs(encounters) do
        local includeEncounter = true
        
        -- Apply time modifiers for undead
        if enc.tag == "UNDEAD" then
            local roll = math.random()
            if roll > (timeData.undeadChance or 1.0) then
                includeEncounter = false
            end
        end
        
        if includeEncounter then
            table.insert(filteredEncounters, enc)
        end
    end
    
    -- If all encounters filtered out, use original list
    if #filteredEncounters == 0 then
        filteredEncounters = encounters
    end
    
    -- Select encounter
    local monsterType = self:selectFromWeighted(filteredEncounters, playerLevel)
    
    if not monsterType then
        log("WARNING: No valid encounters for level " .. playerLevel .. " in " .. location)
        return nil
    end
    
    -- Calculate monster level (±3 from player level)
    local levelVariance = math.random(-3, 3)
    local monsterLevel = math.max(1, math.min(20, playerLevel + levelVariance))
    
    return {
        monsterType = monsterType,
        monsterLevel = monsterLevel,
        location = location,
        timeOfDay = timeOfDay,
        rarity = rarity
    }
end

-- Check encounter chance (roll every X seconds)
function EncounterTables:shouldSpawnEncounter(location, timeSinceLastEncounter)
    -- Base spawn chance: 20% per minute
    local baseChancePerMinute = 20
    local minutes = timeSinceLastEncounter / 60
    local chance = baseChancePerMinute * minutes
    
    -- Cap at 80%
    chance = math.min(80, chance)
    
    local roll = math.random(1, 100)
    return roll <= chance
end

-- Get all available locations
function EncounterTables:getLocations()
    local locs = {}
    for name, data in pairs(self.locations) do
        table.insert(locs, {name = name, description = data.description})
    end
    return locs
end

-- API for external use
function EncounterTables:getEncounter(location, hour, playerLevel)
    local timeOfDay = self:getTimeOfDay(hour or 12)
    return self:rollEncounter(location, timeOfDay, playerLevel)
end

return EncounterTables
