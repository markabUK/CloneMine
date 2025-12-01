-- Summon Powersets: Pet and minion summoning powersets
-- Loaded dynamically by the ClassSystem

return {
    -- Necromancy Powerset
    {
        name = "Necromancy",
        description = "Raise undead minions to fight for you",
        category = "SUMMON",
        damageType = "NECROTIC",
        maxPets = 4,
        abilities = {
            { id = 6001, name = "Raise Skeleton", level = 1, summon = "skeleton_warrior", cooldown = 5.0, duration = 300.0, maxCount = 2 },
            { id = 6002, name = "Raise Zombie", level = 4, summon = "zombie", cooldown = 8.0, duration = 300.0, maxCount = 2 },
            { id = 6003, name = "Summon Skeleton Mage", level = 8, summon = "skeleton_mage", cooldown = 15.0, duration = 300.0, maxCount = 1 },
            { id = 6004, name = "Summon Bone Golem", level = 12, summon = "bone_golem", cooldown = 60.0, duration = 120.0, maxCount = 1 },
            { id = 6005, name = "Army of the Dead", level = 20, summon = "skeleton_army", cooldown = 300.0, duration = 30.0, summonCount = 8, temporary = true }
        },
        pets = {
            { name = "skeleton_warrior", health = 100, damage = 20, armor = 5, type = "melee" },
            { name = "zombie", health = 150, damage = 15, armor = 3, type = "melee", slow = true },
            { name = "skeleton_mage", health = 60, damage = 35, armor = 0, type = "ranged", damageType = "SHADOW" },
            { name = "bone_golem", health = 500, damage = 40, armor = 20, type = "melee", taunt = true },
            { name = "skeleton_army", health = 50, damage = 15, armor = 0, type = "melee" }
        }
    },
    
    -- Conjuration Powerset
    {
        name = "Conjuration",
        description = "Summon arcane constructs and elementals",
        category = "SUMMON",
        damageType = "ARCANE",
        maxPets = 3,
        abilities = {
            { id = 6101, name = "Summon Arcane Familiar", level = 1, summon = "arcane_familiar", cooldown = 10.0, duration = 600.0, maxCount = 1, permanent = true },
            { id = 6102, name = "Summon Water Elemental", level = 4, summon = "water_elemental", cooldown = 20.0, duration = 300.0, maxCount = 1 },
            { id = 6103, name = "Conjure Mana Wyrm", level = 8, summon = "mana_wyrm", cooldown = 15.0, duration = 180.0, maxCount = 2 },
            { id = 6104, name = "Mirror Image", level = 12, summon = "mirror_image", cooldown = 120.0, duration = 30.0, summonCount = 3, copyAbilities = true },
            { id = 6105, name = "Summon Arcane Giant", level = 20, summon = "arcane_giant", cooldown = 180.0, duration = 60.0, maxCount = 1 }
        },
        pets = {
            { name = "arcane_familiar", health = 80, damage = 25, armor = 0, type = "ranged", damageType = "ARCANE", manaRegen = true },
            { name = "water_elemental", health = 200, damage = 30, armor = 5, type = "ranged", damageType = "WATER", freeze = true },
            { name = "mana_wyrm", health = 60, damage = 20, armor = 0, type = "ranged", damageType = "ARCANE" },
            { name = "mirror_image", health = 1, damage = 15, armor = 0, type = "ranged", damageType = "ARCANE", untargetable = true },
            { name = "arcane_giant", health = 800, damage = 60, armor = 15, type = "melee", damageType = "ARCANE" }
        }
    },
    
    -- Beast Mastery Powerset
    {
        name = "Beast Mastery",
        description = "Command wild beasts and nature's creatures",
        category = "SUMMON",
        damageType = "NATURE",
        maxPets = 3,
        abilities = {
            { id = 6201, name = "Call Wolf", level = 1, summon = "wolf", cooldown = 10.0, duration = 600.0, maxCount = 1, permanent = true },
            { id = 6202, name = "Summon Bear", level = 4, summon = "bear", cooldown = 20.0, duration = 300.0, maxCount = 1 },
            { id = 6203, name = "Call of the Wild", level = 8, summon = "wild_pack", cooldown = 60.0, duration = 30.0, summonCount = 3 },
            { id = 6204, name = "Summon Dire Beast", level = 12, summon = "dire_beast", cooldown = 45.0, duration = 15.0, maxCount = 1, damageBoost = true },
            { id = 6205, name = "Stampede", level = 20, summon = "beast_stampede", cooldown = 300.0, duration = 20.0, summonCount = 5 }
        },
        pets = {
            { name = "wolf", health = 120, damage = 25, armor = 5, type = "melee", howl = true },
            { name = "bear", health = 300, damage = 20, armor = 15, type = "melee", taunt = true },
            { name = "wild_pack", health = 60, damage = 15, armor = 3, type = "melee" },
            { name = "dire_beast", health = 200, damage = 50, armor = 8, type = "melee" },
            { name = "beast_stampede", health = 100, damage = 30, armor = 0, type = "melee", knockback = true }
        }
    },
    
    -- Demon Summoning Powerset
    {
        name = "Demon Summoning",
        description = "Call forth demons from the nether realms",
        category = "SUMMON",
        damageType = "SHADOW",
        maxPets = 2,
        abilities = {
            { id = 6301, name = "Summon Imp", level = 1, summon = "imp", cooldown = 10.0, duration = 600.0, maxCount = 1, permanent = true },
            { id = 6302, name = "Summon Voidwalker", level = 4, summon = "voidwalker", cooldown = 20.0, duration = 600.0, maxCount = 1, permanent = true },
            { id = 6303, name = "Summon Succubus", level = 8, summon = "succubus", cooldown = 20.0, duration = 600.0, maxCount = 1, permanent = true },
            { id = 6304, name = "Summon Felguard", level = 12, summon = "felguard", cooldown = 30.0, duration = 600.0, maxCount = 1, permanent = true },
            { id = 6305, name = "Summon Infernal", level = 20, summon = "infernal", cooldown = 180.0, duration = 30.0, maxCount = 1, impactDamage = 500 }
        },
        pets = {
            { name = "imp", health = 50, damage = 30, armor = 0, type = "ranged", damageType = "FIRE", manaRegen = true },
            { name = "voidwalker", health = 400, damage = 15, armor = 20, type = "melee", taunt = true, sacrifice = true },
            { name = "succubus", health = 100, damage = 35, armor = 0, type = "ranged", charm = true, whip = true },
            { name = "felguard", health = 250, damage = 45, armor = 10, type = "melee", intercept = true },
            { name = "infernal", health = 600, damage = 60, armor = 25, type = "melee", damageType = "FIRE", aoe = true }
        }
    },
    
    -- Elemental Summoning Powerset
    {
        name = "Elemental Summoning",
        description = "Call forth primal elementals to do your bidding",
        category = "SUMMON",
        damageType = "NATURE",
        maxPets = 2,
        abilities = {
            { id = 6401, name = "Summon Earth Elemental", level = 1, summon = "earth_elemental", cooldown = 30.0, duration = 300.0, maxCount = 1 },
            { id = 6402, name = "Summon Fire Elemental", level = 4, summon = "fire_elemental", cooldown = 30.0, duration = 300.0, maxCount = 1 },
            { id = 6403, name = "Summon Storm Elemental", level = 8, summon = "storm_elemental", cooldown = 30.0, duration = 300.0, maxCount = 1 },
            { id = 6404, name = "Primal Elementalist", level = 12, buff = "empower_elemental", cooldown = 60.0, duration = 20.0, empowerAmount = 0.50 },
            { id = 6405, name = "Elemental Mastery", level = 20, summon = "all_elementals", cooldown = 300.0, duration = 30.0, summonCount = 3 }
        },
        pets = {
            { name = "earth_elemental", health = 500, damage = 25, armor = 30, type = "melee", taunt = true },
            { name = "fire_elemental", health = 200, damage = 50, armor = 5, type = "ranged", damageType = "FIRE" },
            { name = "storm_elemental", health = 150, damage = 40, armor = 5, type = "ranged", damageType = "LIGHTNING" },
            { name = "all_elementals", health = 300, damage = 40, armor = 15, type = "mixed" }
        }
    }
}
