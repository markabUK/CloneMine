-- Monster Abilities (Separate from Player Abilities)
-- Abilities used by NPCs and monsters only

return {
    -- Basic Monster Abilities
    {
        name = "Bite",
        type = "MELEE_ATTACK",
        damage = 15,
        cooldown = 0,
        range = 5,
        description = "Basic bite attack used by beasts"
    },
    
    {
        name = "Claw",
        type = "MELEE_ATTACK",
        damage = 12,
        cooldown = 0,
        range = 5,
        bleed = {duration = 6, tickDamage = 3},
        description = "Claw attack that causes bleeding"
    },
    
    {
        name = "Tail Sweep",
        type = "AOE_ATTACK",
        damage = 20,
        cooldown = 8,
        range = 8,
        aoe = {radius = 5, behindCaster = true},
        knockback = 10,
        description = "Sweeps tail to hit enemies behind"
    },
    
    -- Pack Tactics
    {
        name = "Pack Call",
        type = "SUMMON",
        cooldown = 60,
        summonCount = 2,
        summonType = "WOLF",
        description = "Calls 2 wolves to assist"
    },
    
    {
        name = "Alpha Howl",
        type = "BUFF",
        cooldown = 30,
        duration = 15,
        aoe = {radius = 20, allyOnly = true},
        damageBonus = 25,
        attackSpeedBonus = 20,
        description = "Buffs all nearby pack members"
    },
    
    -- Elite Monster Abilities
    {
        name = "Enrage",
        type = "BUFF",
        cooldown = 45,
        duration = 20,
        triggerAtLowHealth = 30,  -- Triggers at 30% health
        damageBonus = 50,
        attackSpeedBonus = 40,
        damageReduction = 30,
        description = "Becomes enraged at low health"
    },
    
    {
        name = "Summon Adds",
        type = "SUMMON",
        cooldown = 90,
        summonCount = 3,
        summonType = "MINION",
        triggerAtHealthThreshold = {25, 50, 75},
        description = "Summons adds at specific health thresholds"
    },
    
    {
        name = "Ground Slam",
        type = "AOE_ATTACK",
        damage = 40,
        cooldown = 15,
        aoe = {radius = 10},
        stun = 3,
        description = "Slams ground, stunning all nearby enemies"
    },
    
    {
        name = "Charge",
        type = "MELEE_ATTACK",
        damage = 35,
        cooldown = 20,
        range = 25,
        chargeToTarget = true,
        knockdown = true,
        description = "Charges at target, knocking them down"
    },
    
    -- Boss Abilities
    {
        name = "Phase Transition",
        type = "PHASE_CHANGE",
        cooldown = 0,
        triggerAtHealthPercent = {66, 33},
        healPercent = 10,
        clearThreat = true,
        summonGuards = 2,
        description = "Transitions to next phase at health thresholds"
    },
    
    {
        name = "Meteor Strike",
        type = "AOE_ATTACK",
        damage = 80,
        cooldown = 30,
        castTime = 3.0,
        range = 40,
        aoe = {radius = 12},
        telegraphed = true,
        description = "Calls down a meteor (telegraphed attack)"
    },
    
    {
        name = "Mind Control",
        type = "CROWD_CONTROL",
        cooldown = 45,
        duration = 10,
        range = 30,
        crowdControl = "CHARM",
        description = "Takes control of a player"
    },
    
    {
        name = "Death Aura",
        type = "AOE_DOT",
        cooldown = 0,  // Always active
        aoe = {radius = 15},
        tickDamage = 10,
        tickInterval = 2,
        description = "Constant aura that damages nearby players"
    },
    
    {
        name = "Summon Infernals",
        type = "SUMMON",
        cooldown = 60,
        summonCount = 4,
        summonType = "INFERNAL",
        summonLocation = "RANDOM_IN_AREA",
        description = "Summons infernals at random locations"
    },
    
    {
        name = "Fel Cannon",
        type = "RANGED_ATTACK",
        damage = 60,
        cooldown = 12,
        range = 50,
        projectileSpeed = "FAST",
        aoe = {radius = 5},
        description = "Fires a fel projectile"
    },
    
    {
        name = "Resurrect Allies",
        type = "HEAL_SUMMON",
        cooldown = 90,
        range = 30,
        resurrectDeadAllies = true,
        maxResurrections = 2,
        description = "Resurrects up to 2 dead allies"
    },
    
    {
        name = "Curse of Weakness",
        type = "DEBUFF",
        cooldown = 20,
        duration = 30,
        range = 30,
        damageReduction = 50,
        description = "Reduces target's damage by 50%"
    },
    
    {
        name = "Teleport",
        type = "MOBILITY",
        cooldown = 25,
        range = 40,
        escapeAtLowHealth = 20,  // Teleports when below 20% health
        description = "Teleports to a random location"
    },
    
    {
        name = "Shield Wall",
        type = "BUFF",
        cooldown = 60,
        duration = 15,
        damageReduction = 75,
        reflectDamage = 25,
        description = "Massive damage reduction with reflect"
    },
    
    {
        name = "Frenzy Strike",
        type = "MELEE_ATTACK",
        damage = 25,
        cooldown = 5,
        hitCount = 5,  // Hits 5 times rapidly
        attackSpeedBonus = 100,
        description = "Rapid series of strikes"
    },
    
    {
        name = "Poison Cloud",
        type = "AOE_DOT",
        cooldown = 25,
        duration = 12,
        aoe = {radius = 10},
        tickDamage = 8,
        tickInterval = 2,
        persistent = true,  // Cloud persists on ground
        description = "Creates a lingering poison cloud"
    },
    
    {
        name = "Life Drain Aura",
        type = "AOE_DOT",
        cooldown = 45,
        duration = 10,
        aoe = {radius = 20},
        tickDamage = 15,
        healing = 15,  // Heals caster for damage dealt
        tickInterval = 1,
        description = "Drains life from all nearby enemies"
    },
    
    {
        name = "Web Trap",
        type = "CROWD_CONTROL",
        cooldown = 30,
        duration = 8,
        range = 25,
        crowdControl = "ROOT",
        aoe = {radius = 5},
        description = "Traps targets in webs"
    },
    
    {
        name = "Burrow",
        type = "MOBILITY",
        cooldown = 40,
        duration = 5,
        untargetable = true,
        healsWhileBurrowed = 5,  // Percentage per second
        description = "Burrows underground, becoming untargetable"
    },
    
    {
        name = "Rage",
        type = "BUFF",
        cooldown = 0,  // Triggered ability
        triggerOnDamageTaken = 100,  // Triggers after taking 100 damage
        duration = 8,
        damageBonus = 30,
        description = "Becomes enraged after taking damage"
    },
    
    {
        name = "Cleave",
        type = "AOE_ATTACK",
        damage = 30,
        cooldown = 6,
        range = 5,
        aoe = {radius = 5, cone = true, angle = 90},
        description = "Cleaves in a cone in front"
    },
    
    {
        name = "Breath Weapon",
        type = "AOE_ATTACK",
        damage = 70,
        cooldown = 20,
        castTime = 2.0,
        range = 30,
        aoe = {radius = 8, cone = true, angle = 60},
        damageType = "FIRE",  // Or FROST, POISON, etc.
        description = "Dragon breath weapon"
    },
    
    {
        name = "Wing Buffet",
        type = "AOE_ATTACK",
        damage = 25,
        cooldown = 15,
        aoe = {radius = 15},
        knockback = 20,
        description = "Knocks back all nearby enemies with wings"
    },
    
    {
        name = "Fear Aura",
        type = "AOE_CROWD_CONTROL",
        cooldown = 60,
        duration = 6,
        aoe = {radius = 20},
        crowdControl = "FEAR",
        description = "Causes all nearby enemies to flee in terror"
    }
}
