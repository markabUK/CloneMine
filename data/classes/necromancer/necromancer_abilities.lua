-- Necromancer Abilities
-- Non-spell abilities for commanding undead and manipulating corpses

return {
    -- Level 1 Starting Ability
    {
        name = "Raise Dead",
        level = 1,
        manaCost = 40,
        castTime = 3.0,
        cooldown = 10,
        range = 10,
        requiresCorpse = true,
        summonType = "SKELETON",
        description = "Raises a skeleton from a nearby corpse.",
        effects = {
            summon = "SKELETON_MINION",
            duration = 0,  -- Permanent until dismissed or killed
            consumesCorpse = true
        }
    },
    
    -- Level 2
    {
        name = "Command Undead",
        level = 2,
        manaCost = 0,
        castTime = 0,
        cooldown = 1,
        range = 50,
        description = "Command your undead minions to attack a specific target.",
        effects = {
            commandType = "ATTACK",
            targetType = "ENEMY"
        }
    },
    
    {
        name = "Recall Undead",
        level = 2,
        manaCost = 0,
        castTime = 0,
        cooldown = 5,
        range = 0,
        description = "Recall all your undead minions to your side.",
        effects = {
            commandType = "RETURN",
            teleport = true
        }
    },
    
    -- Level 4
    {
        name = "Sacrifice Minion",
        level = 4,
        manaCost = 0,
        castTime = 1.0,
        cooldown = 30,
        range = 30,
        description = "Sacrifice an undead minion to heal yourself.",
        effects = {
            sacrificeMinion = true,
            healing = 50,  -- Percentage of your max health
            manaRestore = 30  -- Percentage of your max mana
        }
    },
    
    -- Level 6
    {
        name = "Raise Zombie",
        level = 6,
        manaCost = 60,
        castTime = 4.0,
        cooldown = 15,
        range = 10,
        requiresCorpse = true,
        summonType = "ZOMBIE",
        description = "Raises a tougher zombie from a corpse.",
        effects = {
            summon = "ZOMBIE_TANK",
            duration = 0,  -- Permanent
            consumesCorpse = true
        }
    },
    
    -- Level 10
    {
        name = "Dark Pact",
        level = 10,
        manaCost = 0,
        castTime = 0,
        cooldown = 60,
        range = 0,
        description = "Sacrifice 20% of your health to restore 40% of your mana.",
        effects = {
            healthCost = 20,  -- Percentage
            manaRestore = 40  -- Percentage
        }
    },
    
    {
        name = "Unholy Power",
        level = 10,
        manaCost = 50,
        castTime = 0,
        cooldown = 120,
        range = 0,
        buff = {
            duration = 20,
            spellPower = 30,
            cooldownReduction = 20  -- Percentage
        },
        description = "Temporarily increases your spell power and reduces cooldowns."
    },
    
    -- Level 12
    {
        name = "Mass Raise Dead",
        level = 12,
        manaCost = 80,
        castTime = 5.0,
        cooldown = 60,
        range = 20,
        requiresCorpses = 3,
        description = "Raises up to 3 skeletons from nearby corpses at once.",
        effects = {
            summon = "SKELETON_MINION",
            count = 3,
            duration = 0,  -- Permanent
            consumesCorpses = 3
        }
    },
    
    -- Level 15
    {
        name = "Raise Ghoul",
        level = 15,
        manaCost = 100,
        castTime = 6.0,
        cooldown = 30,
        range = 10,
        requiresCorpse = true,
        summonType = "GHOUL",
        description = "Raises a powerful ghoul from a corpse.",
        effects = {
            summon = "GHOUL_ELITE",
            duration = 0,  -- Permanent
            consumesCorpse = true
        }
    },
    
    {
        name = "Empower Undead",
        level = 15,
        manaCost = 60,
        castTime = 0,
        cooldown = 90,
        range = 0,
        buff = {
            duration = 30,
            target = "ALL_UNDEAD_MINIONS",
            damageBonus = 50,  -- Percentage
            healthBonus = 30,  -- Percentage
            attackSpeed = 25   // Percentage
        },
        description = "Empower all your undead minions, greatly increasing their power."
    },
    
    -- Level 18
    {
        name = "Soul Harvest",
        level = 18,
        manaCost = 0,
        castTime = 0,
        cooldown = 0,
        range = 10,
        passive = false,
        description = "When an enemy dies near you, gain a soul charge (max 5). Each charge increases spell power by 5%.",
        effects = {
            onEnemyDeath = true,
            maxCharges = 5,
            spellPowerPerCharge = 5,
            chargeDuration = 30
        }
    },
    
    -- Level 20 (Ultimate)
    {
        name = "Become Death",
        level = 20,
        manaCost = 200,
        castTime = 3.0,
        cooldown = 600,
        range = 0,
        description = "Transform into the embodiment of death for 45 seconds.",
        effects = {
            transform = "DEATH_INCARNATE",
            duration = 45,
            buffs = {
                spellPower = 100,  -- Percentage increase
                armor = 50,
                allSpellsCostZero = true,
                allCooldownsHalved = true,
                lifeDrainAura = 10,  -- Drain 10 HP/s from all enemies within 15 yards
                undead MinionsDoubled = true
            }
        }
    }
}
