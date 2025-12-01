-- Attack Powersets: Damage dealing powersets for all schools of magic
-- Loaded dynamically by the ClassSystem

return {
    -- Fire Attack Powerset
    {
        name = "Fire Blast",
        description = "Unleash devastating fire attacks that burn enemies to ashes",
        category = "ATTACK",
        damageType = "FIRE",
        abilities = {
            { id = 1001, name = "Fireball", level = 1, damage = 50, cooldown = 2.0 },
            { id = 1002, name = "Flame Strike", level = 4, damage = 80, cooldown = 4.0 },
            { id = 1003, name = "Immolate", level = 8, damage = 30, cooldown = 1.0, isDot = true, duration = 6.0 },
            { id = 1004, name = "Fire Breath", level = 12, damage = 120, cooldown = 8.0, isAoe = true },
            { id = 1005, name = "Meteor Storm", level = 20, damage = 200, cooldown = 30.0, isAoe = true }
        }
    },
    
    -- Ice Attack Powerset
    {
        name = "Ice Assault",
        description = "Freeze and shatter foes with powerful ice magic",
        category = "ATTACK",
        damageType = "ICE",
        abilities = {
            { id = 1101, name = "Ice Bolt", level = 1, damage = 45, cooldown = 2.0 },
            { id = 1102, name = "Frost Nova", level = 4, damage = 60, cooldown = 6.0, isAoe = true },
            { id = 1103, name = "Blizzard", level = 8, damage = 40, cooldown = 10.0, isAoe = true, duration = 5.0 },
            { id = 1104, name = "Ice Lance", level = 12, damage = 100, cooldown = 3.0 },
            { id = 1105, name = "Frozen Orb", level = 20, damage = 180, cooldown = 25.0, isAoe = true }
        }
    },
    
    -- Lightning Attack Powerset
    {
        name = "Lightning Strike",
        description = "Strike enemies with the raw power of lightning",
        category = "ATTACK",
        damageType = "LIGHTNING",
        abilities = {
            { id = 1201, name = "Shock", level = 1, damage = 40, cooldown = 1.5 },
            { id = 1202, name = "Chain Lightning", level = 4, damage = 70, cooldown = 5.0, chainTargets = 3 },
            { id = 1203, name = "Thunderbolt", level = 8, damage = 90, cooldown = 4.0 },
            { id = 1204, name = "Storm Call", level = 12, damage = 130, cooldown = 12.0, isAoe = true },
            { id = 1205, name = "Thunder God's Wrath", level = 20, damage = 220, cooldown = 35.0, isAoe = true }
        }
    },
    
    -- Arcane Attack Powerset
    {
        name = "Arcane Power",
        description = "Channel pure arcane energy to devastate opponents",
        category = "ATTACK",
        damageType = "ARCANE",
        abilities = {
            { id = 1301, name = "Arcane Missile", level = 1, damage = 35, cooldown = 1.0 },
            { id = 1302, name = "Arcane Blast", level = 4, damage = 75, cooldown = 3.0 },
            { id = 1303, name = "Arcane Barrage", level = 8, damage = 55, cooldown = 2.0 },
            { id = 1304, name = "Arcane Explosion", level = 12, damage = 90, cooldown = 6.0, isAoe = true },
            { id = 1305, name = "Arcane Overload", level = 20, damage = 250, cooldown = 40.0 }
        }
    },
    
    -- Shadow Attack Powerset
    {
        name = "Shadow Strike",
        description = "Embrace the darkness and strike from the shadows",
        category = "ATTACK",
        damageType = "SHADOW",
        abilities = {
            { id = 1401, name = "Shadow Bolt", level = 1, damage = 45, cooldown = 2.0 },
            { id = 1402, name = "Dark Tendril", level = 4, damage = 65, cooldown = 4.0 },
            { id = 1403, name = "Shadowflame", level = 8, damage = 25, cooldown = 1.5, isDot = true, duration = 8.0 },
            { id = 1404, name = "Void Eruption", level = 12, damage = 110, cooldown = 10.0, isAoe = true },
            { id = 1405, name = "Death's Embrace", level = 20, damage = 190, cooldown = 30.0 }
        }
    },
    
    -- Necrotic Attack Powerset
    {
        name = "Necrotic Touch",
        description = "Wield the power of death itself against your enemies",
        category = "ATTACK",
        damageType = "NECROTIC",
        abilities = {
            { id = 1501, name = "Touch of Death", level = 1, damage = 40, cooldown = 2.0 },
            { id = 1502, name = "Soul Drain", level = 4, damage = 50, cooldown = 3.0, lifesteal = 0.3 },
            { id = 1503, name = "Corpse Explosion", level = 8, damage = 100, cooldown = 8.0, isAoe = true },
            { id = 1504, name = "Plague", level = 12, damage = 20, cooldown = 2.0, isDot = true, duration = 12.0 },
            { id = 1505, name = "Army of the Dead", level = 20, damage = 180, cooldown = 60.0, summons = true }
        }
    },
    
    -- Holy Attack Powerset
    {
        name = "Holy Smite",
        description = "Burn the unholy with divine wrath",
        category = "ATTACK",
        damageType = "HOLY",
        abilities = {
            { id = 1601, name = "Smite", level = 1, damage = 50, cooldown = 2.0 },
            { id = 1602, name = "Holy Fire", level = 4, damage = 70, cooldown = 4.0 },
            { id = 1603, name = "Consecration", level = 8, damage = 30, cooldown = 10.0, isAoe = true, duration = 8.0 },
            { id = 1604, name = "Divine Storm", level = 12, damage = 100, cooldown = 8.0, isAoe = true },
            { id = 1605, name = "Wrath of the Heavens", level = 20, damage = 200, cooldown = 30.0, isAoe = true }
        }
    },
    
    -- Acid Attack Powerset
    {
        name = "Acid Spray",
        description = "Dissolve enemies with corrosive acid attacks",
        category = "ATTACK",
        damageType = "ACID",
        abilities = {
            { id = 1701, name = "Acid Splash", level = 1, damage = 35, cooldown = 1.5 },
            { id = 1702, name = "Corrode", level = 4, damage = 20, cooldown = 2.0, isDot = true, duration = 10.0 },
            { id = 1703, name = "Acid Rain", level = 8, damage = 60, cooldown = 8.0, isAoe = true },
            { id = 1704, name = "Melt Armor", level = 12, damage = 40, cooldown = 6.0, debuff = "armor_reduction" },
            { id = 1705, name = "Caustic Torrent", level = 20, damage = 160, cooldown = 25.0, isAoe = true }
        }
    },
    
    -- Water Attack Powerset
    {
        name = "Water Torrent",
        description = "Command the seas to crush your foes",
        category = "ATTACK",
        damageType = "WATER",
        abilities = {
            { id = 1801, name = "Water Jet", level = 1, damage = 40, cooldown = 1.5 },
            { id = 1802, name = "Tidal Wave", level = 4, damage = 75, cooldown = 6.0, isAoe = true },
            { id = 1803, name = "Whirlpool", level = 8, damage = 50, cooldown = 8.0, isAoe = true, duration = 5.0 },
            { id = 1804, name = "Tsunami", level = 12, damage = 120, cooldown = 15.0, isAoe = true },
            { id = 1805, name = "Wrath of the Deep", level = 20, damage = 200, cooldown = 35.0, isAoe = true }
        }
    },
    
    -- Psionic Attack Powerset
    {
        name = "Psionic Assault",
        description = "Shatter minds with pure psychic power",
        category = "ATTACK",
        damageType = "PSIONIC",
        abilities = {
            { id = 1901, name = "Mind Blast", level = 1, damage = 45, cooldown = 2.0 },
            { id = 1902, name = "Psychic Scream", level = 4, damage = 60, cooldown = 5.0, isAoe = true, fear = true },
            { id = 1903, name = "Mental Anguish", level = 8, damage = 25, cooldown = 1.5, isDot = true, duration = 8.0 },
            { id = 1904, name = "Brain Storm", level = 12, damage = 100, cooldown = 10.0, isAoe = true },
            { id = 1905, name = "Mind Crush", level = 20, damage = 180, cooldown = 30.0 }
        }
    },
    
    -- Nature Attack Powerset
    {
        name = "Nature's Wrath",
        description = "Call upon nature's fury to destroy enemies",
        category = "ATTACK",
        damageType = "NATURE",
        abilities = {
            { id = 2001, name = "Thorn Strike", level = 1, damage = 40, cooldown = 1.5 },
            { id = 2002, name = "Entangle", level = 4, damage = 30, cooldown = 4.0, root = true, duration = 3.0 },
            { id = 2003, name = "Insect Swarm", level = 8, damage = 20, cooldown = 2.0, isDot = true, duration = 10.0 },
            { id = 2004, name = "Hurricane", level = 12, damage = 90, cooldown = 12.0, isAoe = true },
            { id = 2005, name = "Force of Nature", level = 20, damage = 170, cooldown = 30.0, summons = true }
        }
    },
    
    -- Physical Piercing Attack Powerset
    {
        name = "Piercing Strikes",
        description = "Penetrate armor with precise piercing attacks",
        category = "ATTACK",
        damageType = "PIERCING",
        abilities = {
            { id = 2101, name = "Thrust", level = 1, damage = 45, cooldown = 1.0 },
            { id = 2102, name = "Impale", level = 4, damage = 80, cooldown = 4.0 },
            { id = 2103, name = "Perforate", level = 8, damage = 35, cooldown = 2.0, armorPen = 0.5 },
            { id = 2104, name = "Skewer", level = 12, damage = 100, cooldown = 6.0 },
            { id = 2105, name = "Death Stab", level = 20, damage = 180, cooldown = 20.0 }
        }
    },
    
    -- Physical Slashing Attack Powerset
    {
        name = "Slashing Fury",
        description = "Carve through enemies with devastating slashing attacks",
        category = "ATTACK",
        damageType = "SLASHING",
        abilities = {
            { id = 2201, name = "Cleave", level = 1, damage = 50, cooldown = 1.5, isAoe = true },
            { id = 2202, name = "Rend", level = 4, damage = 20, cooldown = 2.0, isDot = true, duration = 6.0 },
            { id = 2203, name = "Whirlwind", level = 8, damage = 70, cooldown = 6.0, isAoe = true },
            { id = 2204, name = "Blade Storm", level = 12, damage = 110, cooldown = 10.0, isAoe = true },
            { id = 2205, name = "Executioner's Swing", level = 20, damage = 200, cooldown = 25.0 }
        }
    },
    
    -- Physical Blunt Attack Powerset
    {
        name = "Crushing Blows",
        description = "Smash enemies with overwhelming blunt force",
        category = "ATTACK",
        damageType = "BLUNT",
        abilities = {
            { id = 2301, name = "Bash", level = 1, damage = 55, cooldown = 1.5 },
            { id = 2302, name = "Concussive Strike", level = 4, damage = 70, cooldown = 4.0, stun = 2.0 },
            { id = 2303, name = "Ground Pound", level = 8, damage = 80, cooldown = 6.0, isAoe = true },
            { id = 2304, name = "Hammer of Judgement", level = 12, damage = 120, cooldown = 8.0 },
            { id = 2305, name = "Cataclysm", level = 20, damage = 220, cooldown = 30.0, isAoe = true }
        }
    }
}
