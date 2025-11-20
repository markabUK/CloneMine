-- Monsters and NPCs Plugin
-- Defines monsters to fight and quest-giving NPCs

-- Monster Types
local MonsterType = {
    BEAST = "Beast",
    UNDEAD = "Undead",
    DEMON = "Demon",
    ELEMENTAL = "Elemental",
    HUMANOID = "Humanoid",
    DRAGON = "Dragon",
    GIANT = "Giant"
}

-- Monster Difficulty
local Difficulty = {
    TRIVIAL = 1,
    EASY = 2,
    NORMAL = 3,
    HARD = 4,
    ELITE = 5,
    BOSS = 6,
    RAID_BOSS = 7
}

-- Monster definitions
local Monsters = {
    -- Low Level (1-10)
    WOLF = {
        name = "Wolf",
        level = 2,
        type = MonsterType.BEAST,
        difficulty = Difficulty.NORMAL,
        health = 50,
        damage = 5,
        armor = 2,
        xpReward = 25,
        goldReward = {min = 1, max = 3},
        loot = {
            {item = "wolf_pelt", chance = 0.3},
            {item = "meat", chance = 0.5}
        },
        abilities = {"Bite"},
        behavior = "aggressive",
        packSize = {min = 1, max = 3}
    },
    
    GOBLIN = {
        name = "Goblin",
        level = 3,
        type = MonsterType.HUMANOID,
        difficulty = Difficulty.NORMAL,
        health = 40,
        damage = 6,
        armor = 3,
        xpReward = 30,
        goldReward = {min = 2, max = 5},
        loot = {
            {item = "goblin_ear", chance = 0.4},
            {item = "rusty_dagger", chance = 0.1}
        },
        abilities = {"Stab", "Throw Rock"},
        behavior = "aggressive",
        packSize = {min = 2, max = 5}
    },
    
    ZOMBIE = {
        name = "Zombie",
        level = 5,
        type = MonsterType.UNDEAD,
        difficulty = Difficulty.NORMAL,
        health = 80,
        damage = 8,
        armor = 1,
        xpReward = 50,
        goldReward = {min = 3, max = 7},
        loot = {
            {item = "rotten_flesh", chance = 0.8},
            {item = "bone", chance = 0.3}
        },
        abilities = {"Slam", "Infect"},
        behavior = "aggressive",
        weaknesses = {"HOLY", "FIRE"},
        resistances = {"SHADOW", "FROST"}
    },
    
    SPIDER = {
        name = "Giant Spider",
        level = 4,
        type = MonsterType.BEAST,
        difficulty = Difficulty.NORMAL,
        health = 60,
        damage = 7,
        armor = 4,
        xpReward = 40,
        goldReward = {min = 2, max = 4},
        loot = {
            {item = "spider_silk", chance = 0.6},
            {item = "poison_gland", chance = 0.2}
        },
        abilities = {"Bite", "Web", "Poison"},
        behavior = "ambush"
    },
    
    ORC_WARRIOR = {
        name = "Orc Warrior",
        level = 8,
        type = MonsterType.HUMANOID,
        difficulty = Difficulty.NORMAL,
        health = 120,
        damage = 12,
        armor = 8,
        xpReward = 80,
        goldReward = {min = 5, max = 12},
        loot = {
            {item = "iron_sword", chance = 0.15},
            {item = "orc_tooth", chance = 0.4},
            {item = "leather_armor", chance = 0.1}
        },
        abilities = {"Cleave", "Battle Shout"},
        behavior = "aggressive",
        packSize = {min = 2, max = 4}
    },
    
    -- Mid Level (11-20)
    TROLL = {
        name = "Troll",
        level = 12,
        type = MonsterType.GIANT,
        difficulty = Difficulty.HARD,
        health = 200,
        damage = 18,
        armor = 10,
        xpReward = 150,
        goldReward = {min = 10, max = 20},
        loot = {
            {item = "troll_blood", chance = 0.5},
            {item = "large_club", chance = 0.2}
        },
        abilities = {"Crush", "Regeneration"},
        behavior = "aggressive",
        regeneration = 5  -- HP per second
    },
    
    FIRE_ELEMENTAL = {
        name = "Fire Elemental",
        level = 15,
        type = MonsterType.ELEMENTAL,
        difficulty = Difficulty.HARD,
        health = 180,
        damage = 22,
        armor = 5,
        xpReward = 200,
        goldReward = {min = 8, max = 15},
        loot = {
            {item = "fire_essence", chance = 0.7},
            {item = "ember_core", chance = 0.3}
        },
        abilities = {"Flame Burst", "Immolate", "Fire Shield"},
        behavior = "aggressive",
        immunities = {"FIRE"},
        weaknesses = {"FROST", "WATER"},
        aura = {type = "fire_damage", radius = 5, damage = 3}
    },
    
    NECROMANCER = {
        name = "Necromancer",
        level = 18,
        type = MonsterType.HUMANOID,
        difficulty = Difficulty.ELITE,
        health = 250,
        mana = 300,
        damage = 15,
        armor = 8,
        spellpower = 40,
        xpReward = 300,
        goldReward = {min = 15, max = 30},
        loot = {
            {item = "necromantic_staff", chance = 0.1},
            {item = "dark_robe", chance = 0.15},
            {item = "soul_gem", chance = 0.4}
        },
        abilities = {
            "Shadow Bolt", "Fear", "Drain Life", 
            "Summon Skeleton", "Death Coil"
        },
        behavior = "caster",
        summons = {"SKELETON"},
        immunities = {"SHADOW"}
    },
    
    // High Level (21-30)
    WYVERN = {
        name = "Wyvern",
        level = 25,
        type = MonsterType.DRAGON,
        difficulty = Difficulty.ELITE,
        health = 400,
        damage = 35,
        armor = 15,
        xpReward = 500,
        goldReward = {min = 25, max = 50},
        loot = {
            {item = "wyvern_scale", chance = 0.6},
            {item = "wyvern_sting", chance = 0.3},
            {item = "rare_gem", chance = 0.1}
        },
        abilities = {"Tail Lash", "Aerial Strike", "Poison Cloud", "Wing Buffet"},
        behavior = "flying",
        canFly = true,
        resistances = {"PHYSICAL"}
    },
    
    DEMON_LORD = {
        name = "Demon Lord",
        level = 28,
        type = MonsterType.DEMON,
        difficulty = Difficulty.ELITE,
        health = 500,
        mana = 400,
        damage = 40,
        armor = 20,
        spellpower = 60,
        xpReward = 700,
        goldReward = {min = 40, max = 80},
        loot = {
            {item = "demonic_essence", chance = 0.8},
            {item = "infernal_weapon", chance = 0.15},
            {item = "demon_soul", chance = 0.2}
        },
        abilities = {
            "Chaos Bolt", "Immolate", "Hellfire", 
            "Summon Imp", "Dark Pact", "Demon Charge"
        },
        behavior = "aggressive",
        summons = {"IMP", "VOIDWALKER"},
        immunities = {"FIRE", "SHADOW"},
        aura = {type = "corruption", radius = 10, damagePerTick = 5}
    },
    
    -- Bosses (30+)
    ANCIENT_DRAKE = {
        name = "Ancient Drake",
        level = 35,
        type = MonsterType.DRAGON,
        difficulty = Difficulty.BOSS,
        health = 2000,
        mana = 500,
        damage = 60,
        armor = 30,
        spellpower = 80,
        xpReward = 2000,
        goldReward = {min = 100, max = 200},
        loot = {
            {item = "drake_scale_armor", chance = 0.3},
            {item = "drake_fang_sword", chance = 0.2},
            {item = "ancient_gem", chance = 0.5},
            {item = "epic_ring", chance = 0.15}
        },
        abilities = {
            "Frost Breath", "Tail Swipe", "Wing Storm",
            "Ice Prison", "Blizzard", "Frightful Presence"
        },
        behavior = "boss",
        canFly = true,
        phases = {
            {healthPercent = 100, abilities = {"Frost Breath", "Tail Swipe"}},
            {healthPercent = 60, abilities = {"Blizzard", "Ice Prison"}, summons = {"ICE_ELEMENTAL"}},
            {healthPercent = 30, abilities = {"Wing Storm", "Frightful Presence"}, enrage = true}
        },
        immunities = {"FROST"},
        resistances = {"PHYSICAL", "ARCANE"}
    },
    
    LICH_KING = {
        name = "The Lich King",
        level = 40,
        type = MonsterType.UNDEAD,
        difficulty = Difficulty.RAID_BOSS,
        health = 5000,
        mana = 2000,
        damage = 80,
        armor = 40,
        spellpower = 120,
        xpReward = 5000,
        goldReward = {min = 500, max = 1000},
        loot = {
            {item = "frostmourne", chance = 0.05},  -- Legendary weapon
            {item = "crown_of_dominion", chance = 0.1},
            {item = "legendary_armor_piece", chance = 0.2},
            {item = "soul_of_the_damned", chance = 0.8}
        },
        abilities = {
            "Death Coil", "Army of the Dead", "Remorseless Winter",
            "Soul Reaper", "Summon Val'kyr", "Harvest Soul",
            "Fury of Frostmourne", "Defile"
        },
        behavior = "raid_boss",
        requiresRaid = true,
        phases = {
            {healthPercent = 100, abilities = {"Death Coil", "Soul Reaper"}},
            {healthPercent = 70, abilities = {"Army of the Dead", "Remorseless Winter"}, 
             summons = {"DEATH_KNIGHT", "GHOUL"}},
            {healthPercent = 40, abilities = {"Summon Val'kyr", "Harvest Soul"}, 
             platformMechanic = true},
            {healthPercent = 10, abilities = {"Fury of Frostmourne", "Defile"}, 
             enrage = true, damageMultiplier = 2.0}
        },
        immunities = {"SHADOW", "FROST"},
        resistances = {"HOLY", "FIRE", "PHYSICAL"}
    }
}

-- Quest definitions
local Quests = {
    -- Starter Quests
    WOLF_PROBLEM = {
        id = 1,
        name = "Wolf Problem",
        description = "The village is being terrorized by wolves. Kill 10 wolves.",
        level = 1,
        questGiver = "Village Elder",
        objectives = {
            {type = "kill", target = "WOLF", count = 10}
        },
        rewards = {
            xp = 200,
            gold = 50,
            items = {{item = "leather_boots", quantity = 1}}
        },
        nextQuest = 2
    },
    
    GOBLIN_THREAT = {
        id = 2,
        name = "Goblin Threat",
        description = "Goblins have been stealing supplies. Defeat 15 goblins and retrieve 5 stolen items.",
        level = 3,
        questGiver = "Village Elder",
        prerequisites = {1},
        objectives = {
            {type = "kill", target = "GOBLIN", count = 15},
            {type = "collect", item = "stolen_supplies", count = 5}
        },
        rewards = {
            xp = 400,
            gold = 100,
            items = {{item = "iron_sword", quantity = 1}}
        }
    },
    
    UNDEAD_RISING = {
        id = 3,
        name = "The Undead Rising",
        description = "Zombies are rising from the graveyard. Investigate and destroy 20 zombies.",
        level = 5,
        questGiver = "Town Guard Captain",
        objectives = {
            {type = "kill", target = "ZOMBIE", count = 20},
            {type = "explore", location = "Old Graveyard"}
        },
        rewards = {
            xp = 600,
            gold = 150,
            items = {
                {item = "silver_mace", quantity = 1},
                {item = "holy_water", quantity = 3}
            }
        },
        nextQuest = 4
    },
    
    NECROMANCER_PLOT = {
        id = 4,
        name = "The Necromancer's Plot",
        description = "A necromancer is behind the undead attacks. Defeat him and bring back his staff.",
        level = 18,
        questGiver = "Town Guard Captain",
        prerequisites = {3},
        objectives = {
            {type = "kill", target = "NECROMANCER", count = 1},
            {type = "collect", item = "necromantic_staff", count = 1}
        },
        rewards = {
            xp = 3000,
            gold = 500,
            items = {
                {item = "blessed_armor", quantity = 1},
                {item = "anti-undead_charm", quantity = 1}
            }
        },
        questChain = "Undead Menace"
    },
    
    -- Mid-Level Quests
    TROLL_HUNT = {
        id = 5,
        name = "Troll Hunt",
        description = "Trolls are attacking travelers. Hunt down and kill 5 trolls.",
        level = 12,
        questGiver = "Bounty Master",
        objectives = {
            {type = "kill", target = "TROLL", count = 5}
        },
        rewards = {
            xp = 1500,
            gold = 300,
            items = {{item = "troll_hide_armor", quantity = 1}}
        }
    },
    
    ELEMENTAL_IMBALANCE = {
        id = 6,
        name = "Elemental Imbalance",
        description = "Fire elementals are running rampant. Defeat 10 and collect their essences.",
        level = 15,
        questGiver = "Archmage",
        objectives = {
            {type = "kill", target = "FIRE_ELEMENTAL", count = 10},
            {type = "collect", item = "fire_essence", count = 7}
        },
        rewards = {
            xp = 2000,
            gold = 400,
            items = {
                {item = "fire_resist_cloak", quantity = 1},
                {item = "elemental_staff", quantity = 1}
            }
        }
    },
    
    -- High-Level Quests
    WYVERN_MENACE = {
        id = 7,
        name = "Wyvern Menace",
        description = "Wyverns are attacking aerial routes. Defeat the alpha wyvern.",
        level = 25,
        questGiver = "Flight Master",
        objectives = {
            {type = "kill", target = "WYVERN", count = 1, variant = "alpha"}
        },
        rewards = {
            xp = 5000,
            gold = 800,
            items = {
                {item = "wyvern_scale_mail", quantity = 1},
                {item = "flight_speed_boots", quantity = 1}
            }
        }
    },
    
    DEMON_INVASION = {
        id = 8,
        name = "Demon Invasion",
        description = "A demon lord is opening a portal. Stop the invasion!",
        level = 28,
        questGiver = "High Priest",
        objectives = {
            {type = "kill", target = "DEMON_LORD", count = 1},
            {type = "close", object = "demon_portal", count = 1}
        },
        rewards = {
            xp = 7000,
            gold = 1000,
            items = {
                {item = "demon_slayer_weapon", quantity = 1},
                {item = "holy_relic", quantity = 1}
            }
        },
        questChain = "Burning Crusade"
    },
    
    -- Epic Boss Quests
    DRAKE_CHALLENGE = {
        id = 9,
        name = "Challenge of the Ancient Drake",
        description = "Face the ancient drake in its lair and claim its treasure.",
        level = 35,
        questGiver = "Dragon Hunter",
        difficulty = "Epic",
        objectives = {
            {type = "kill", target = "ANCIENT_DRAKE", count = 1}
        },
        rewards = {
            xp = 20000,
            gold = 5000,
            items = {
                {item = "drake_slayer_title", quantity = 1},
                {item = "epic_weapon_of_choice", quantity = 1}
            },
            reputation = {faction = "Dragon Hunters", amount = 1000}
        }
    },
    
    DEFEAT_LICH_KING = {
        id = 10,
        name = "Fall of the Lich King",
        description = "Assemble a raid and defeat the Lich King once and for all.",
        level = 40,
        questGiver = "King of Stormwind",
        difficulty = "Legendary",
        requiresRaid = true,
        objectives = {
            {type = "kill", target = "LICH_KING", count = 1}
        },
        rewards = {
            xp = 50000,
            gold = 10000,
            items = {
                {item = "legendary_weapon", quantity = 1},
                {item = "kingslayer_title", quantity = 1}
            },
            reputation = {faction = "Alliance", amount = 5000},
            achievementUnlock = "Savior of Azeroth"
        }
    }
}

function onLoad()
    log("Monsters and Quests Plugin loaded!")
    
    local monsterCount = 0
    local questCount = 0
    local bosses = 0
    
    for _ in pairs(Monsters) do
        monsterCount = monsterCount + 1
    end
    
    for _ in pairs(Quests) do
        questCount = questCount + 1
    end
    
    for _, monster in pairs(Monsters) do
        if monster.difficulty >= Difficulty.BOSS then
            bosses = bosses + 1
        end
    end
    
    log("Total monsters: " .. monsterCount)
    log("Boss encounters: " .. bosses)
    log("Total quests: " .. questCount)
end

-- Get monster by name
function getMonster(monsterName)
    return Monsters[monsterName]
end

-- Get quest by ID
function getQuest(questId)
    return Quests[questId]
end

-- Get quests available for level
function getQuestsForLevel(playerLevel)
    local available = {}
    for questId, quest in pairs(Quests) do
        if playerLevel >= quest.level then
            table.insert(available, quest)
        end
    end
    return available
end

-- Export
_G.Monsters = Monsters
_G.Quests = Quests
_G.getMonster = getMonster
_G.getQuest = getQuest
_G.getQuestsForLevel = getQuestsForLevel
