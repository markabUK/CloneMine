-- Monster Spells (Separate from Player Spells)
-- Spells used by NPCs and monsters only

return {
    {
        name = "Dark Blast",
        school = "Shadow",
        manaCost = 0,  -- Monsters don't use mana
        castTime = 1.5,
        cooldown = 5,
        range = 30,
        damage = 40,
        damageType = "Shadow",
        description = "Dark energy blast used by shadow creatures"
    },
    
    {
        name = "Poison Spit",
        school = "Nature",
        castTime = 1.0,
        cooldown = 8,
        range = 25,
        damage = 20,
        damageType = "Poison",
        dot = {duration = 12, tickDamage = 5, tickInterval = 3},
        description = "Spits poison at target"
    },
    
    {
        name = "Fireball Barrage",
        school = "Fire",
        castTime = 3.0,
        cooldown = 20,
        range = 40,
        projectileCount = 5,
        damage = 30,  // Per projectile
        damageType = "Fire",
        description = "Launches multiple fireballs"
    },
    
    {
        name = "Frost Nova",
        school = "Frost",
        castTime = 0,
        cooldown = 25,
        range = 0,
        aoe = {radius = 10},
        damage = 35,
        damageType = "Frost",
        crowdControl = "Root",
        duration = 4,
        description = "Freezes all nearby enemies in place"
    },
    
    {
        name = "Chain Lightning",
        school = "Nature",
        castTime = 2.0,
        cooldown = 15,
        range = 35,
        damage = 45,
        damageType = "Lightning",
        chainTargets = 3,
        chainReduction = 25,  // Each chain deals 25% less
        description = "Lightning that chains to multiple targets"
    },
    
    {
        name = "Curse of Agony",
        school = "Shadow",
        castTime = 1.5,
        cooldown = 10,
        range = 30,
        dot = {duration = 18, tickDamage = 8, tickInterval = 3},
        description = "Afflicts target with agony over time"
    },
    
    {
        name = "Mass Dispel",
        school = "Arcane",
        castTime = 2.0,
        cooldown = 30,
        range = 40,
        aoe = {radius = 15},
        removesBuffs = 2,
        description = "Removes buffs from all enemies in area"
    },
    
    {
        name = "Shadowbolt Volley",
        school = "Shadow",
        castTime = 2.5,
        cooldown = 12,
        range = 35,
        aoe = {radius = 10},
        damage = 50,
        damageType = "Shadow",
        description = "Volleys shadowbolts at all targets in area"
    },
    
    {
        name = "Arcane Missiles",
        school = "Arcane",
        castTime = 3.0,
        cooldown = 0,
        range = 40,
        channeled = true,
        missileCount = 5,
        damage = 15,  // Per missile
        damageType = "Arcane",
        description = "Channel to fire arcane missiles"
    },
    
    {
        name = "Blight",
        school = "Disease",
        castTime = 2.0,
        cooldown = 30,
        range = 35,
        aoe = {radius = 12},
        dot = {duration = 15, tickDamage = 10, tickInterval = 3},
        spreadable = true,
        description = "Infectious disease that can spread"
    },
    
    {
        name = "Mind Blast",
        school = "Shadow",
        castTime = 1.5,
        cooldown = 8,
        range = 30,
        damage = 55,
        damageType = "Shadow",
        silenceDuration = 3,
        description = "Blasts the mind, dealing damage and silencing"
    },
    
    {
        name = "Meteor Storm",
        school = "Fire",
        castTime = 4.0,
        cooldown = 60,
        range = 50,
        aoe = {radius = 20},
        duration = 10,
        tickDamage = 25,
        tickInterval = 2,
        damageType = "Fire",
        description = "Calls down meteors in an area"
    },
    
    {
        name = "Death and Decay",
        school = "Death",
        castTime = 2.0,
        cooldown = 30,
        range = 35,
        aoe = {radius = 12},
        duration = 10,
        tickDamage = 18,
        tickInterval = 1,
        damageType = "Shadow",
        description = "Corrupts an area with death magic"
    },
    
    {
        name = "Lightning Storm",
        school = "Nature",
        castTime = 3.0,
        cooldown = 45,
        range = 0,
        aoe = {radius = 30},
        strikeCount = 8,
        damage = 40,  // Per strike
        damageType = "Lightning",
        randomTargets = true,
        description = "Summons a storm that strikes random enemies"
    },
    
    {
        name = "Void Blast",
        school = "Shadow",
        castTime = 2.5,
        cooldown = 20,
        range = 40,
        damage = 80,
        damageType = "Shadow",
        armorReduction = 50,  // Reduces armor for 10 seconds
        description = "Powerful void magic that also reduces armor"
    }
}
