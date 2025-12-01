-- Defense Powersets: Damage mitigation and protection powersets
-- Loaded dynamically by the ClassSystem

return {
    -- Ice Armor Defense Powerset
    {
        name = "Ice Armor",
        description = "Encase yourself in protective ice that shields against damage",
        category = "DEFENSE",
        damageType = "ICE",
        abilities = {
            { id = 3001, name = "Frost Shield", level = 1, absorb = 100, cooldown = 15.0, duration = 10.0 },
            { id = 3002, name = "Ice Barrier", level = 4, absorb = 200, cooldown = 30.0, duration = 20.0 },
            { id = 3003, name = "Frozen Fortitude", level = 8, damageReduction = 0.20, cooldown = 60.0, duration = 15.0 },
            { id = 3004, name = "Glacial Presence", level = 12, absorb = 350, cooldown = 45.0, duration = 20.0, reflectDamage = 0.1 },
            { id = 3005, name = "Avatar of Winter", level = 20, damageReduction = 0.50, cooldown = 180.0, duration = 20.0 }
        }
    },
    
    -- Radiant Shield Defense Powerset
    {
        name = "Radiant Shield",
        description = "Create barriers of pure light to protect yourself and allies",
        category = "DEFENSE",
        damageType = "RADIANT",
        abilities = {
            { id = 3101, name = "Light Barrier", level = 1, absorb = 120, cooldown = 15.0, duration = 10.0 },
            { id = 3102, name = "Divine Protection", level = 4, damageReduction = 0.15, cooldown = 25.0, duration = 12.0 },
            { id = 3103, name = "Aegis of Light", level = 8, absorb = 250, cooldown = 40.0, duration = 15.0, isAoe = true },
            { id = 3104, name = "Guardian Angel", level = 12, absorb = 400, cooldown = 60.0, duration = 20.0, healOnBreak = 100 },
            { id = 3105, name = "Invincibility", level = 20, immune = true, cooldown = 300.0, duration = 8.0 }
        }
    },
    
    -- Stone Skin Defense Powerset
    {
        name = "Stone Skin",
        description = "Turn your body to stone for incredible durability",
        category = "DEFENSE",
        damageType = "NATURE",
        abilities = {
            { id = 3201, name = "Stone Form", level = 1, armorBonus = 50, cooldown = 20.0, duration = 15.0 },
            { id = 3202, name = "Granite Shield", level = 4, damageReduction = 0.25, cooldown = 30.0, duration = 10.0 },
            { id = 3203, name = "Earth's Embrace", level = 8, absorb = 300, cooldown = 45.0, duration = 20.0 },
            { id = 3204, name = "Living Mountain", level = 12, armorBonus = 150, cooldown = 60.0, duration = 30.0 },
            { id = 3205, name = "Diamond Skin", level = 20, damageReduction = 0.60, cooldown = 180.0, duration = 15.0 }
        }
    },
    
    -- Willpower Defense Powerset
    {
        name = "Willpower",
        description = "Use pure mental fortitude to resist damage and regenerate health",
        category = "DEFENSE",
        damageType = "PSIONIC",
        abilities = {
            { id = 3301, name = "Mental Barrier", level = 1, absorb = 80, cooldown = 12.0, duration = 8.0 },
            { id = 3302, name = "Regeneration", level = 4, healPerSecond = 20, cooldown = 30.0, duration = 15.0 },
            { id = 3303, name = "Indomitable Will", level = 8, damageReduction = 0.20, cooldown = 40.0, duration = 12.0 },
            { id = 3304, name = "Mind Over Matter", level = 12, absorb = 400, cooldown = 60.0, duration = 20.0 },
            { id = 3305, name = "Transcendence", level = 20, healPerSecond = 60, damageReduction = 0.30, cooldown = 120.0, duration = 20.0 }
        }
    },
    
    -- Dark Regeneration Defense Powerset
    {
        name = "Dark Regeneration",
        description = "Draw on dark energies to heal and protect yourself",
        category = "DEFENSE",
        damageType = "SHADOW",
        abilities = {
            { id = 3401, name = "Shadow Veil", level = 1, damageReduction = 0.10, cooldown = 15.0, duration = 10.0 },
            { id = 3402, name = "Dark Renewal", level = 4, healPerSecond = 25, cooldown = 25.0, duration = 12.0 },
            { id = 3403, name = "Embrace the Void", level = 8, absorb = 200, cooldown = 35.0, duration = 15.0, lifesteal = 0.2 },
            { id = 3404, name = "Death's Door", level = 12, immune = true, cooldown = 180.0, duration = 5.0, triggerOnLowHealth = true },
            { id = 3405, name = "Lord of Shadows", level = 20, damageReduction = 0.40, healPerSecond = 40, cooldown = 120.0, duration = 20.0 }
        }
    },
    
    -- Holy Defense Powerset
    {
        name = "Holy Bulwark",
        description = "Divine protection shields you from harm",
        category = "DEFENSE",
        damageType = "HOLY",
        abilities = {
            { id = 3501, name = "Blessed Shield", level = 1, absorb = 100, cooldown = 15.0, duration = 10.0 },
            { id = 3502, name = "Divine Favor", level = 4, damageReduction = 0.15, cooldown = 25.0, duration = 15.0 },
            { id = 3503, name = "Guardian's Light", level = 8, absorb = 250, cooldown = 40.0, duration = 20.0, reflectDamage = 0.15 },
            { id = 3504, name = "Sanctuary", level = 12, immune = true, cooldown = 240.0, duration = 6.0 },
            { id = 3505, name = "Divine Intervention", level = 20, absorb = 800, damageReduction = 0.50, cooldown = 300.0, duration = 15.0 }
        }
    }
}
