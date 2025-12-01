-- Control Powersets: Crowd control and debuff powersets
-- Loaded dynamically by the ClassSystem

return {
    -- Mind Control Powerset
    {
        name = "Mind Control",
        description = "Dominate the minds of your enemies",
        category = "CONTROL",
        damageType = "PSIONIC",
        abilities = {
            { id = 7001, name = "Mesmerize", level = 1, effect = "stun", duration = 3.0, cooldown = 8.0 },
            { id = 7002, name = "Confusion", level = 4, effect = "confuse", duration = 10.0, cooldown = 15.0 },
            { id = 7003, name = "Mass Hypnosis", level = 8, effect = "sleep", duration = 8.0, cooldown = 30.0, isAoe = true },
            { id = 7004, name = "Dominate", level = 12, effect = "charm", duration = 20.0, cooldown = 60.0 },
            { id = 7005, name = "Total Domination", level = 20, effect = "charm", duration = 30.0, cooldown = 120.0, isAoe = true, maxTargets = 3 }
        }
    },
    
    -- Ice Control Powerset
    {
        name = "Ice Control",
        description = "Freeze and slow your enemies with ice magic",
        category = "CONTROL",
        damageType = "ICE",
        abilities = {
            { id = 7101, name = "Frost Bolt", level = 1, effect = "slow", slowAmount = 0.30, duration = 6.0, cooldown = 0, damage = 20 },
            { id = 7102, name = "Freeze", level = 4, effect = "root", duration = 5.0, cooldown = 10.0, damage = 40 },
            { id = 7103, name = "Ice Patch", level = 8, effect = "slow", slowAmount = 0.50, duration = 15.0, cooldown = 20.0, isAoe = true },
            { id = 7104, name = "Glacier", level = 12, effect = "stun", duration = 4.0, cooldown = 25.0, isAoe = true, damage = 80 },
            { id = 7105, name = "Arctic Prison", level = 20, effect = "frozen", duration = 8.0, cooldown = 90.0, immune = true }
        }
    },
    
    -- Shadow Binding Powerset
    {
        name = "Shadow Binding",
        description = "Trap enemies in shadowy restraints",
        category = "CONTROL",
        damageType = "SHADOW",
        abilities = {
            { id = 7201, name = "Shadow Grasp", level = 1, effect = "slow", slowAmount = 0.40, duration = 5.0, cooldown = 6.0 },
            { id = 7202, name = "Tenebrous Tentacles", level = 4, effect = "root", duration = 4.0, cooldown = 12.0, damage = 30 },
            { id = 7203, name = "Darkness", level = 8, effect = "blind", duration = 8.0, cooldown = 20.0, isAoe = true },
            { id = 7204, name = "Shadow Prison", level = 12, effect = "stun", duration = 5.0, cooldown = 25.0, damage = 60 },
            { id = 7205, name = "Void Zone", level = 20, effect = "fear", duration = 6.0, cooldown = 60.0, isAoe = true, damage = 40, isDot = true }
        }
    },
    
    -- Nature Control Powerset
    {
        name = "Nature's Grasp",
        description = "Entangle and slow enemies with nature's power",
        category = "CONTROL",
        damageType = "NATURE",
        abilities = {
            { id = 7301, name = "Entangle", level = 1, effect = "root", duration = 4.0, cooldown = 8.0 },
            { id = 7302, name = "Creeping Vines", level = 4, effect = "slow", slowAmount = 0.50, duration = 10.0, cooldown = 15.0, isAoe = true },
            { id = 7303, name = "Spore Cloud", level = 8, effect = "confuse", duration = 6.0, cooldown = 20.0, isAoe = true },
            { id = 7304, name = "Living Vines", level = 12, effect = "root", duration = 8.0, cooldown = 25.0, isAoe = true, damage = 50 },
            { id = 7305, name = "Force of Nature", level = 20, effect = "knockdown", cooldown = 60.0, isAoe = true, summonTreants = 3 }
        }
    },
    
    -- Gravity Control Powerset
    {
        name = "Gravity Control",
        description = "Manipulate gravity to control the battlefield",
        category = "CONTROL",
        damageType = "ARCANE",
        abilities = {
            { id = 7401, name = "Gravity Well", level = 1, effect = "slow", slowAmount = 0.40, duration = 6.0, cooldown = 10.0, pullIn = true },
            { id = 7402, name = "Lift", level = 4, effect = "lift", duration = 4.0, cooldown = 12.0, damage = 30 },
            { id = 7403, name = "Wormhole", level = 8, effect = "teleport", cooldown = 30.0, isAoe = true, range = 40 },
            { id = 7404, name = "Crushing Gravity", level = 12, effect = "slow", slowAmount = 0.80, damage = 80, cooldown = 20.0, isAoe = true },
            { id = 7405, name = "Black Hole", level = 20, effect = "stun", duration = 6.0, cooldown = 120.0, isAoe = true, pullIn = true, damage = 150 }
        }
    },
    
    -- Fire Control Powerset
    {
        name = "Fire Control",
        description = "Control enemies with fear of fire",
        category = "CONTROL",
        damageType = "FIRE",
        abilities = {
            { id = 7501, name = "Ring of Fire", level = 1, effect = "root", duration = 5.0, cooldown = 10.0, isAoe = true },
            { id = 7502, name = "Flashfire", level = 4, effect = "stun", duration = 2.0, cooldown = 15.0, isAoe = true },
            { id = 7503, name = "Cinders", level = 8, effect = "slow", slowAmount = 0.40, duration = 10.0, cooldown = 8.0, isDot = true, damage = 20 },
            { id = 7504, name = "Hot Feet", level = 12, effect = "fear", duration = 4.0, cooldown = 25.0, isAoe = true, damage = 60 },
            { id = 7505, name = "Bonfire", level = 20, effect = "knockback", damage = 120, cooldown = 60.0, isAoe = true, stun = 3.0 }
        }
    }
}
