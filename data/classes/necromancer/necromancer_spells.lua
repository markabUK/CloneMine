-- Necromancer Spells
-- Death magic and life drain spells

return {
    -- Level 1 Starting Spells
    {
        name = "Death Bolt",
        school = "Shadow",
        level = 1,
        manaCost = 15,
        castTime = 1.5,
        cooldown = 0,
        range = 30,
        damage = 25,
        damageType = "Shadow",
        description = "Hurls a bolt of death energy at the target."
    },
    
    {
        name = "Drain Life",
        school = "Shadow",
        level = 1,
        manaCost = 20,
        castTime = 2.0,
        cooldown = 6,
        range = 30,
        damage = 30,
        damageType = "Shadow",
        healing = 30,  -- Heals caster for damage dealt
        description = "Drains life from the target, healing you for the damage dealt."
    },
    
    -- Level 3
    {
        name = "Plague Touch",
        school = "Disease",
        level = 3,
        manaCost = 25,
        castTime = 1.0,
        cooldown = 10,
        range = 5,
        damage = 15,
        damageType = "Disease",
        dot = {duration = 12, tickDamage = 5, tickInterval = 3},
        description = "Infects the target with a deadly plague that deals damage over time."
    },
    
    -- Level 5
    {
        name = "Bone Shield",
        school = "Death",
        level = 5,
        manaCost = 30,
        castTime = 0,
        cooldown = 30,
        range = 0,
        buff = {
            duration = 20,
            armor = 10,
            absorbDamage = 50
        },
        description = "Surrounds you with bones that absorb damage."
    },
    
    {
        name = "Corpse Explosion",
        school = "Death",
        level = 5,
        manaCost = 40,
        castTime = 1.5,
        cooldown = 15,
        range = 30,
        damage = 80,
        damageType = "Physical",
        aoe = {radius = 8},
        requiresCorpse = true,
        description = "Detonates a corpse, dealing damage to all nearby enemies."
    },
    
    -- Level 8
    {
        name = "Death Coil",
        school = "Shadow",
        level = 8,
        manaCost = 35,
        castTime = 0,
        cooldown = 12,
        range = 30,
        damage = 60,
        damageType = "Shadow",
        healing = 60,  -- If cast on undead ally
        description = "Deals shadow damage to an enemy or heals an undead ally."
    },
    
    {
        name = "Fear the Reaper",
        school = "Shadow",
        level = 8,
        manaCost = 30,
        castTime = 1.5,
        cooldown = 20,
        range = 30,
        duration = 4,
        crowdControl = "Fear",
        description = "Strikes fear into the target, causing them to flee in terror."
    },
    
    -- Level 10
    {
        name = "Death and Decay",
        school = "Death",
        level = 10,
        manaCost = 60,
        castTime = 2.0,
        cooldown = 30,
        range = 35,
        aoe = {radius = 10, duration = 10, tickDamage = 15, tickInterval = 1},
        damageType = "Shadow",
        description = "Corrupts the ground, dealing shadow damage to all enemies in the area."
    },
    
    -- Level 12
    {
        name = "Soul Siphon",
        school = "Shadow",
        level = 12,
        manaCost = 45,
        castTime = 3.0,
        cooldown = 20,
        range = 30,
        channeled = true,
        damage = 25,  -- Per second
        damageType = "Shadow",
        healing = 25,  -- Per second
        description = "Channel to continuously drain life from the target."
    },
    
    {
        name = "Unholy Frenzy",
        school = "Death",
        level = 12,
        manaCost = 40,
        castTime = 0,
        cooldown = 60,
        range = 30,
        buff = {
            duration = 15,
            attackSpeed = 50,  -- Percentage increase
            castSpeed = 30,
            healthDrainPerSecond = 2  -- Costs health
        },
        description = "Increases attack and cast speed but drains health over time."
    },
    
    -- Level 15
    {
        name = "Lich Form",
        school = "Death",
        level = 15,
        manaCost = 80,
        castTime = 2.0,
        cooldown = 180,
        range = 0,
        buff = {
            duration = 30,
            spellPower = 50,
            armorBonus = 20,
            manaRegenBonus = 10,
            transform = "LICH"
        },
        description = "Transform into a lich, greatly enhancing your necromantic powers."
    },
    
    {
        name = "Army of the Dead",
        school = "Death",
        level = 15,
        manaCost = 100,
        castTime = 3.0,
        cooldown = 300,
        range = 0,
        summons = {
            type = "TEMPORARY_SKELETONS",
            count = 8,
            duration = 40
        },
        description = "Summons an army of temporary skeletons to fight for you."
    },
    
    -- Level 18
    {
        name = "Death Grip",
        school = "Death",
        level = 18,
        manaCost = 25,
        castTime = 0,
        cooldown = 25,
        range = 30,
        pull = true,
        taunt = true,
        description = "Pulls an enemy to you and forces them to attack you."
    },
    
    -- Level 20 (Ultimate)
    {
        name = "Apocalypse",
        school = "Death",
        level = 20,
        manaCost = 150,
        castTime = 4.0,
        cooldown = 600,
        range = 40,
        aoe = {radius = 20},
        damage = 300,
        damageType = "Shadow",
        dot = {duration = 15, tickDamage = 20, tickInterval = 3},
        summons = {
            type = "DEATH_KNIGHTS",
            count = 4,
            duration = 60
        },
        description = "Ultimate death magic that devastates an area and summons Death Knights."
    }
}
