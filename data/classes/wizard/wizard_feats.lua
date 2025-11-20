-- Wizard Feats
-- Passive bonuses earned every 2 levels (D&D style)
-- Levels: 2, 4, 6, 8, 10, 12, 14, 16, 18, 20

return {
    -- Feat unlock levels (every 2 levels)
    featLevels = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20},
    
    -- Available feats (player chooses 1 per feat level)
    feats = {
        -- Early Feats (Levels 2-6)
        {
            name = "Spell Focus",
            level = 2,
            description = "Increases spell damage by 5%",
            effect = {
                spellDamageBonus = 0.05
            }
        },
        {
            name = "Arcane Concentration",
            level = 2,
            description = "Reduces spell mana cost by 10%",
            effect = {
                manaCostReduction = 0.10
            }
        },
        {
            name = "Spell Penetration",
            level = 4,
            description = "Your spells ignore 15% of enemy resistances",
            effect = {
                resistancePenetration = 0.15
            }
        },
        {
            name = "Improved Counterspell",
            level = 4,
            description = "Counterspell cooldown reduced by 50%",
            effect = {
                abilityCooldownReduction = {Counterspell = 0.50}
            }
        },
        {
            name = "Arcane Thesis",
            level = 6,
            description = "Choose one spell. It deals 10% more damage and costs 15% less mana",
            effect = {
                type = "SPELL_SPECIALIZATION",
                damageBonus = 0.10,
                costReduction = 0.15
            }
        },
        {
            name = "Quick Cast",
            level = 6,
            description = "Reduces global cooldown by 0.5 seconds",
            effect = {
                globalCooldownReduction = 0.5
            }
        },
        
        -- Mid Feats (Levels 8-14)
        {
            name = "Arcane Potency",
            level = 8,
            description = "Critical strike chance increased by 5%",
            effect = {
                critChanceBonus = 0.05
            }
        },
        {
            name = "Improved Polymorph",
            level = 8,
            description = "Polymorph duration increased by 4 seconds",
            effect = {
                polymorphDurationBonus = 4
            }
        },
        {
            name = "Spell Power",
            level = 10,
            description = "INT increases spell damage by an additional 1% per point",
            effect = {
                intToSpellDamageBonus = 0.01
            }
        },
        {
            name = "Mana Attunement",
            level = 10,
            description = "Maximum mana increased by 15%",
            effect = {
                maxManaBonus = 0.15
            }
        },
        {
            name = "Metamagic Master",
            level = 12,
            description = "10% chance to cast spells instantly",
            effect = {
                instantCastChance = 0.10
            }
        },
        {
            name = "Arcane Flows",
            level = 12,
            description = "Mana regeneration increased by 50%",
            effect = {
                manaRegenBonus = 0.50
            }
        },
        {
            name = "Spell Haste",
            level = 14,
            description = "Casting speed increased by 10%",
            effect = {
                castSpeedBonus = 0.10
            }
        },
        {
            name = "Improved Evocation",
            level = 14,
            description = "Evocation now restores 100% mana and cooldown reduced by 60 seconds",
            effect = {
                evocationRestoreBonus = 0.40,
                abilityCooldownReduction = {Evocation = 60}
            }
        },
        
        -- Advanced Feats (Levels 16-20)
        {
            name = "Arcane Mastery",
            level = 16,
            description = "Critical strikes deal 250% damage instead of 200%",
            effect = {
                critDamageMultiplier = 2.5
            }
        },
        {
            name = "Temporal Adept",
            level = 16,
            description = "Cooldowns reduced by 15%",
            effect = {
                globalCooldownReduction = 0.15
            }
        },
        {
            name = "Archmage's Dominance",
            level = 18,
            description = "All arcane spells deal 15% more damage",
            effect = {
                schoolDamageBonus = {Arcane = 0.15}
            }
        },
        {
            name = "Incanter's Absorption",
            level = 18,
            description = "When you take spell damage, gain 15% of that damage as spell power for 10 seconds",
            effect = {
                type = "DAMAGE_TO_POWER",
                conversionRate = 0.15,
                duration = 10
            }
        },
        {
            name = "Master of Elements",
            level = 20,
            description = "Your spells have a 20% chance to refund their mana cost",
            effect = {
                manaRefundChance = 0.20
            }
        },
        {
            name = "Presence of Mind",
            level = 20,
            description = "Your next spell is instant cast (2 min cooldown)",
            effect = {
                type = "ACTIVE_FEAT",
                cooldown = 120
            }
        }
    }
}
