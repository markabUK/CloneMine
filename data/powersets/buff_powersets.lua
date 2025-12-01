-- Buff Powersets: Enhancement and support powersets
-- Loaded dynamically by the ClassSystem

return {
    -- Arcane Empowerment Powerset
    {
        name = "Arcane Empowerment",
        description = "Enhance allies with arcane power",
        category = "BUFF",
        damageType = "ARCANE",
        abilities = {
            { id = 5001, name = "Arcane Intellect", level = 1, buff = "intellect", amount = 10, cooldown = 0, duration = 1800.0 },
            { id = 5002, name = "Time Warp", level = 4, buff = "haste", amount = 0.30, cooldown = 300.0, duration = 15.0, isAoe = true },
            { id = 5003, name = "Focus Magic", level = 8, buff = "crit", amount = 0.05, cooldown = 30.0, duration = 300.0 },
            { id = 5004, name = "Arcane Power", level = 12, buff = "damage", amount = 0.20, cooldown = 90.0, duration = 15.0, selfOnly = true },
            { id = 5005, name = "Temporal Shield", level = 20, buff = "damage_absorb", amount = 0.15, cooldown = 60.0, duration = 12.0, healOnExpire = true }
        }
    },
    
    -- Holy Aura Powerset
    {
        name = "Holy Aura",
        description = "Inspire allies with divine blessings",
        category = "BUFF",
        damageType = "HOLY",
        abilities = {
            { id = 5101, name = "Blessing of Might", level = 1, buff = "strength", amount = 10, cooldown = 0, duration = 1800.0 },
            { id = 5102, name = "Devotion Aura", level = 4, buff = "armor", amount = 50, cooldown = 0, duration = 0, isAura = true },
            { id = 5103, name = "Blessing of Kings", level = 8, buff = "all_stats", amount = 0.10, cooldown = 0, duration = 1800.0 },
            { id = 5104, name = "Crusader Aura", level = 12, buff = "movement_speed", amount = 0.20, cooldown = 0, duration = 0, isAura = true },
            { id = 5105, name = "Avenging Wrath", level = 20, buff = "damage", amount = 0.30, buff2 = "healing", amount2 = 0.30, cooldown = 120.0, duration = 20.0 }
        }
    },
    
    -- Nature's Gift Powerset
    {
        name = "Nature's Gift",
        description = "Bestow nature's blessings upon allies",
        category = "BUFF",
        damageType = "NATURE",
        abilities = {
            { id = 5201, name = "Mark of the Wild", level = 1, buff = "all_stats", amount = 5, cooldown = 0, duration = 1800.0, isAoe = true },
            { id = 5202, name = "Thorns", level = 4, buff = "reflect_damage", amount = 10, cooldown = 60.0, duration = 300.0 },
            { id = 5203, name = "Innervate", level = 8, buff = "mana_regen", amount = 0.50, cooldown = 120.0, duration = 10.0 },
            { id = 5204, name = "Barkskin", level = 12, buff = "damage_reduction", amount = 0.20, cooldown = 60.0, duration = 12.0 },
            { id = 5205, name = "Nature's Swiftness", level = 20, buff = "instant_cast", cooldown = 60.0, duration = 0, nextCast = true }
        }
    },
    
    -- Psionic Boost Powerset
    {
        name = "Psionic Boost",
        description = "Amplify the minds of your allies",
        category = "BUFF",
        damageType = "PSIONIC",
        abilities = {
            { id = 5301, name = "Mind Focus", level = 1, buff = "intellect", amount = 8, buff2 = "wisdom", amount2 = 8, cooldown = 0, duration = 1800.0 },
            { id = 5302, name = "Mental Clarity", level = 4, buff = "mana_cost_reduction", amount = 0.10, cooldown = 60.0, duration = 30.0 },
            { id = 5303, name = "Psychic Link", level = 8, buff = "share_damage", amount = 0.30, cooldown = 30.0, duration = 60.0 },
            { id = 5304, name = "Power Infusion", level = 12, buff = "haste", amount = 0.25, buff2 = "mana_cost_reduction", amount2 = 0.20, cooldown = 120.0, duration = 15.0 },
            { id = 5305, name = "Borrowed Time", level = 20, buff = "absorb_next", amount = 500, cooldown = 90.0, duration = 15.0, isAoe = true }
        }
    },
    
    -- Shadow Empowerment Powerset
    {
        name = "Shadow Empowerment",
        description = "Dark blessings that enhance combat abilities",
        category = "BUFF",
        damageType = "SHADOW",
        abilities = {
            { id = 5401, name = "Dark Intent", level = 1, buff = "haste", amount = 0.05, cooldown = 0, duration = 1800.0 },
            { id = 5402, name = "Shadowfiend Blessing", level = 4, buff = "mana_return", amount = 0.03, cooldown = 180.0, duration = 15.0, perHit = true },
            { id = 5403, name = "Vampiric Embrace", level = 8, buff = "lifesteal", amount = 0.10, cooldown = 60.0, duration = 120.0, selfOnly = true },
            { id = 5404, name = "Dark Soul", level = 12, buff = "crit", amount = 0.20, cooldown = 120.0, duration = 20.0, selfOnly = true },
            { id = 5405, name = "Demonic Pact", level = 20, buff = "spell_power", amount = 0.15, cooldown = 0, duration = 0, isAura = true, requirePet = true }
        }
    }
}
