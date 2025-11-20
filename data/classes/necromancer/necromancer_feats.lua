-- Necromancer Feats (Passive Bonuses and Talents)
-- Talent trees and passive abilities

return {
    -- Passive Bonuses (always active)
    passives = {
        {
            name = "Undead Mastery",
            level = 1,
            description = "Your undead minions have 20% more health and deal 15% more damage.",
            effects = {
                minionHealthBonus = 20,
                minionDamageBonus = 15
            }
        },
        
        {
            name = "Death's Embrace",
            level = 5,
            description = "Life drain effects are 25% more effective.",
            effects = {
                lifeDrainBonus = 25
            }
        },
        
        {
            name = "Corpse Finder",
            level = 8,
            description = "Nearby corpses are highlighted and you can detect them from further away.",
            effects = {
                corpseDetectionRange = 30,
                corpseHighlight = true
            }
        },
        
        {
            name = "Soul Link",
            level = 10,
            description = "30% of damage you take is split among your undead minions.",
            effects = {
                damageSharing = 30
            }
        },
        
        {
            name = "Efficient Necromancy",
            level = 12,
            description = "Death magic spells cost 15% less mana.",
            effects = {
                schoolManaCostReduction = {
                    ["Shadow"] = 15,
                    ["Death"] = 15
                }
            }
        },
        
        {
            name = "Plague Bearer",
            level = 15,
            description = "Your disease effects spread to nearby enemies.",
            effects = {
                diseaseSpreadChance = 30,
                diseaseSpreadRange = 5
            }
        },
        
        {
            name = "Master of Death",
            level = 20,
            description = "Maximum undead minions increased by 1. All death magic deals 20% more damage.",
            effects = {
                maxMinionsBonus = 1,
                deathMagicDamageBonus = 20
            }
        }
    },
    
    -- Talent Trees (choose one path per tree)
    talentTrees = {
        -- Tree 1: Summoning Focus
        summoning = {
            name = "Dark Summoning",
            description = "Enhance your undead summoning capabilities",
            talents = {
                {
                    name = "Rapid Reanimation",
                    level = 5,
                    description = "Raise Dead cast time reduced by 1 second.",
                    effects = {
                        spellCastTimeReduction = {
                            ["Raise Dead"] = 1.0
                        }
                    }
                },
                {
                    name = "Skeletal Archers",
                    level = 10,
                    description = "Your skeletons can use ranged attacks.",
                    effects = {
                        minionRangedAttack = "SKELETON"
                    }
                },
                {
                    name = "Eternal Army",
                    level = 15,
                    description = "Your undead minions take 50% reduced damage.",
                    effects = {
                        minionDamageReduction = 50
                    }
                },
                {
                    name = "Legion of the Damned",
                    level = 20,
                    description = "Army of the Dead summons 12 skeletons instead of 8.",
                    effects = {
                        armyOfDeadCount = 12
                    }
                }
            }
        },
        
        -- Tree 2: Death Magic Focus
        deathMagic = {
            name = "Shadow and Decay",
            description = "Enhance your offensive death magic",
            talents = {
                {
                    name = "Improved Death Bolt",
                    level = 5,
                    description = "Death Bolt has a 15% chance to cost no mana.",
                    effects = {
                        spellFreeCastChance = {
                            ["Death Bolt"] = 15
                        }
                    }
                },
                {
                    name = "Unstable Affliction",
                    level = 10,
                    description = "Your DoT effects deal damage 20% faster.",
                    effects = {
                        dotTickRateBonus = 20
                    }
                },
                {
                    name = "Death's Chill",
                    level = 15,
                    description = "Death magic spells slow enemies by 30% for 3 seconds.",
                    effects = {
                        deathMagicSlow = 30,
                        slowDuration = 3
                    }
                },
                {
                    name = "Reaper's Touch",
                    level = 20,
                    description = "Critical strikes with death magic have a 50% chance to instantly kill enemies below 15% health.",
                    effects = {
                        executeThreshold = 15,
                        executeChance = 50
                    }
                }
            }
        },
        
        -- Tree 3: Survival Focus
        survival = {
            name = "Lichdom",
            description = "Enhance your survivability and sustain",
            talents = {
                {
                    name = "Improved Drain Life",
                    level = 5,
                    description = "Drain Life cooldown reduced by 2 seconds.",
                    effects = {
                        spellCooldownReduction = {
                            ["Drain Life"] = 2.0
                        }
                    }
                },
                {
                    name = "Phylactery",
                    level = 10,
                    description = "When you would die, instead become immune and invisible for 3 seconds (10 minute cooldown).",
                    effects = {
                        cheatDeath = true,
                        cheatDeathCooldown = 600,
                        cheatDeathDuration = 3
                    }
                },
                {
                    name = "Vampiric Aura",
                    level = 15,
                    description = "You and your minions gain 5% life steal on all attacks.",
                    effects = {
                        lifeStealAura = 5
                    }
                },
                {
                    name = "True Immortality",
                    level = 20,
                    description = "Lich Form duration increased to 60 seconds and you become immune to crowd control.",
                    effects = {
                        lichFormDuration = 60,
                        lichFormCCImmune = true
                    }
                }
            }
        }
    },
    
    -- Special Feats (unlock through gameplay)
    specialFeats = {
        {
            name = "Necropolis",
            description = "You can create a Necropolis structure that automatically generates corpses.",
            unlockCondition = "Kill 1000 enemies",
            effects = {
                canBuildNecropolis = true
            }
        },
        {
            name = "Death Knight",
            description = "You can raise fallen enemy heroes as Death Knights.",
            unlockCondition = "Defeat 10 elite enemies",
            effects = {
                canRaiseDeathKnights = true
            }
        },
        {
            name = "Plague Lord",
            description = "Your diseases never expire and stack infinitely.",
            unlockCondition = "Apply 10000 disease damage",
            effects = {
                diseasesNeverExpire = true,
                diseasesStackInfinitely = true
            }
        }
    }
}
