-- Wizard Abilities
-- Active abilities and powers

return {
    abilities = {
        {
            name = "Arcane Intellect",
            level = 1,
            resourceCost = 20,
            cooldown = 0,
            castTime = 0,
            description = "Increases INT by 10% for 30 minutes",
            effect = {
                type = "BUFF",
                duration = 1800,
                statBonus = {INT = 0.10}
            }
        },
        {
            name = "Evocation",
            level = 4,
            resourceCost = 0,
            cooldown = 120,
            castTime = 6,
            description = "Channel for 6 seconds, regenerating 60% of max mana",
            effect = {
                type = "CHANNELED",
                duration = 6,
                manaRestorePercent = 0.60
            }
        },
        {
            name = "Mirror Image",
            level = 8,
            resourceCost = 30,
            cooldown = 180,
            castTime = 0,
            description = "Creates 3 copies of yourself that distract enemies for 40 seconds",
            effect = {
                type = "SUMMON",
                duration = 40,
                count = 3,
                summonType = "MIRROR_IMAGE"
            }
        },
        {
            name = "Mana Shield",
            level = 10,
            resourceCost = 50,
            cooldown = 60,
            castTime = 0,
            description = "Absorbs damage equal to 100% of your mana pool",
            effect = {
                type = "SHIELD",
                duration = 60,
                absorbPercent = 1.0
            }
        },
        {
            name = "Arcane Power",
            level = 14,
            resourceCost = 0,
            cooldown = 90,
            castTime = 0,
            description = "Increases spell damage by 30% but costs 100% more mana for 15 seconds",
            effect = {
                type = "BUFF",
                duration = 15,
                spellDamageBonus = 0.30,
                manaCostIncrease = 1.0
            }
        },
        {
            name = "Spell Steal",
            level = 16,
            resourceCost = 40,
            cooldown = 10,
            castTime = 0,
            description = "Steals a beneficial magic effect from the target",
            effect = {
                type = "DISPEL",
                target = "ENEMY",
                dispelType = "BUFF_STEAL"
            }
        },
        {
            name = "Alter Time",
            level = 18,
            resourceCost = 60,
            cooldown = 180,
            castTime = 0,
            description = "Return to your position and health from 10 seconds ago",
            effect = {
                type = "TIME_REVERT",
                revertTime = 10
            }
        },
        {
            name = "Mass Polymorph",
            level = 20,
            resourceCost = 100,
            cooldown = 300,
            castTime = 2,
            description = "Polymorphs all enemies within 10 yards for 8 seconds",
            effect = {
                type = "CROWD_CONTROL",
                duration = 8,
                aoeRadius = 10,
                ccType = "POLYMORPH"
            }
        }
    }
}
