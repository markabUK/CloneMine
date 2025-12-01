-- Healing Powersets: Health restoration powersets
-- Loaded dynamically by the ClassSystem

return {
    -- Holy Healing Powerset
    {
        name = "Holy Blessing",
        description = "Channel divine energy to heal allies",
        category = "HEALING",
        damageType = "HOLY",
        abilities = {
            { id = 4001, name = "Heal", level = 1, heal = 100, cooldown = 2.0 },
            { id = 4002, name = "Greater Heal", level = 4, heal = 250, cooldown = 4.0 },
            { id = 4003, name = "Renew", level = 8, healPerSecond = 30, cooldown = 8.0, duration = 12.0 },
            { id = 4004, name = "Circle of Healing", level = 12, heal = 150, cooldown = 10.0, isAoe = true },
            { id = 4005, name = "Divine Hymn", level = 20, heal = 200, healPerSecond = 50, cooldown = 60.0, duration = 8.0, isAoe = true }
        }
    },
    
    -- Radiant Restoration Powerset
    {
        name = "Radiant Restoration",
        description = "Bathe allies in healing light",
        category = "HEALING",
        damageType = "RADIANT",
        abilities = {
            { id = 4101, name = "Flash of Light", level = 1, heal = 80, cooldown = 1.5, fast = true },
            { id = 4102, name = "Beacon of Light", level = 4, healPerSecond = 40, cooldown = 15.0, duration = 20.0 },
            { id = 4103, name = "Light Well", level = 8, heal = 60, cooldown = 30.0, duration = 60.0, charges = 10, isAoe = true },
            { id = 4104, name = "Guardian Spirit", level = 12, preventDeath = true, cooldown = 120.0, duration = 10.0, healOnTrigger = 0.5 },
            { id = 4105, name = "Salvation", level = 20, heal = 400, cooldown = 90.0, isAoe = true, removeDebuffs = true }
        }
    },
    
    -- Nature's Embrace Powerset
    {
        name = "Nature's Embrace",
        description = "Call upon nature's healing power",
        category = "HEALING",
        damageType = "NATURE",
        abilities = {
            { id = 4201, name = "Rejuvenation", level = 1, healPerSecond = 25, cooldown = 6.0, duration = 12.0 },
            { id = 4202, name = "Regrowth", level = 4, heal = 120, healPerSecond = 20, cooldown = 4.0, duration = 8.0 },
            { id = 4203, name = "Lifebloom", level = 8, healPerSecond = 35, cooldown = 10.0, duration = 10.0, bloomHeal = 200 },
            { id = 4204, name = "Wild Growth", level = 12, healPerSecond = 25, cooldown = 15.0, duration = 7.0, isAoe = true, maxTargets = 6 },
            { id = 4205, name = "Tranquility", level = 20, healPerSecond = 80, cooldown = 120.0, duration = 8.0, isAoe = true, channeled = true }
        }
    },
    
    -- Water Healing Powerset
    {
        name = "Water Healing",
        description = "Use the life-giving properties of water to restore health",
        category = "HEALING",
        damageType = "WATER",
        abilities = {
            { id = 4301, name = "Healing Wave", level = 1, heal = 90, cooldown = 2.0, chain = 2 },
            { id = 4302, name = "Tidal Surge", level = 4, heal = 180, cooldown = 5.0 },
            { id = 4303, name = "Riptide", level = 8, heal = 60, healPerSecond = 20, cooldown = 6.0, duration = 10.0 },
            { id = 4304, name = "Healing Rain", level = 12, healPerSecond = 30, cooldown = 20.0, duration = 10.0, isAoe = true },
            { id = 4305, name = "Spirit Link", level = 20, redistributeHealth = true, cooldown = 90.0, duration = 6.0, isAoe = true }
        }
    }
}
