-- Complete Stat-Based Damage System
-- Stats affect both damage dealt and damage taken

return {
    -- ========================================
    -- DAMAGE DEALT SCALING (Offensive)
    -- ========================================
    damageDealt = {
        -- Primary offensive stats per class
        primaryStats = {
            STR = {
                physicalDamage = 0.03,      -- +3% physical damage per STR
                criticalDamage = 0.01,      -- +1% crit damage per STR
                armorPenetration = 0.5      -- +0.5 armor penetration per STR
            },
            DEX = {
                physicalDamage = 0.02,      -- +2% physical damage per DEX
                rangedDamage = 0.04,        -- +4% ranged damage per DEX
                criticalChance = 0.1,       -- +0.1% crit chance per DEX
                attackSpeed = 0.05          -- +0.05% attack speed per DEX
            },
            INT = {
                spellDamage = 0.04,         -- +4% spell damage per INT
                spellPenetration = 0.6,     -- +0.6 spell penetration per INT
                manaEfficiency = 0.2,       -- +0.2% mana cost reduction per INT
                dotDamage = 0.02            -- +2% DOT damage per INT
            },
            WIS = {
                spellDamage = 0.03,         -- +3% spell damage per WIS
                healingPower = 0.05,        -- +5% healing per WIS
                manaCost = 0.3,             -- +0.3% mana cost reduction per WIS
                dotDuration = 0.02          -- +2% DOT duration per WIS
            },
            CHA = {
                spellDamage = 0.035,        -- +3.5% spell damage per CHA
                petDamage = 0.06,           -- +6% pet damage per CHA (Mastermind bonus)
                buffDuration = 0.03,        -- +3% buff duration per CHA
                persuasion = 0.5            -- +0.5% merchant discount per CHA
            },
            CON = {
                health = 10,                -- +10 HP per CON
                healthRegen = 0.2,          -- +0.2 HP/sec per CON
                staminaRegen = 0.5          -- +0.5 stamina/sec per CON
            }
        },
        
        -- Secondary offensive stats
        secondaryStats = {
            -- STR also helps with
            STR_secondary = {
                blockPower = 2,             -- +2 block power per STR
                carryWeight = 5             -- +5 carry capacity per STR
            },
            -- DEX also helps with
            DEX_secondary = {
                dodgeChance = 0.08,         -- +0.08% dodge per DEX
                movementSpeed = 0.05        -- +0.05% move speed per DEX
            },
            -- INT also helps with
            INT_secondary = {
                criticalChance = 0.05,      -- +0.05% spell crit per INT (for casters)
                resourcePool = 5            -- +5 mana per INT
            },
            -- WIS also helps with
            WIS_secondary = {
                resourceRegen = 0.1,        -- +0.1 mana regen/sec per WIS
                statusResist = 0.1          -- +0.1% status resistance per WIS
            },
            -- CHA also helps with
            CHA_secondary = {
                summonDuration = 0.02,      -- +2% summon duration per CHA
                resourcePool = 3            -- +3 resource per CHA
            }
        }
    },
    
    -- ========================================
    -- DAMAGE TAKEN REDUCTION (Defensive)
    -- ========================================
    damageTaken = {
        -- Primary defensive stats
        defensiveStats = {
            CON = {
                physicalReduction = 0.05,   -- +0.05% physical damage reduction per CON
                maxHealthIncrease = 0.02,   -- +2% max health per CON
                bleedResist = 0.3,          -- +0.3% bleed resistance per CON
                stunResist = 0.1,           -- +0.1% stun resistance per CON
                maxReductionPercent = 20    -- CAPPED at 20% total reduction from CON
            },
            STR = {
                physicalReduction = 0.02,   -- +0.02% physical damage reduction per STR
                blockChance = 0.05,         -- +0.05% block chance per STR (with shield)
                knockbackResist = 0.4       -- +0.4% knockback resist per STR
            },
            DEX = {
                dodgeChance = 0.1,          -- +0.1% dodge chance per DEX
                parryChance = 0.08,         -- +0.08% parry chance per DEX
                criticalAvoidance = 0.05    -- +0.05% avoid being crit per DEX
            },
            INT = {
                spellReduction = 0.03,      -- +0.03% spell damage reduction per INT
                magicResist = 0.5,          -- +0.5 magic resistance per INT
                silenceResist = 0.2         -- +0.2% silence resistance per INT
            },
            WIS = {
                spellReduction = 0.04,      -- +0.04% spell damage reduction per WIS
                healingReceived = 0.02,     -- +2% healing received per WIS
                debuffDuration = -0.01,     -- -1% debuff duration per WIS
                fearResist = 0.3            -- +0.3% fear resistance per WIS
            },
            CHA = {
                allReduction = 0.01,        -- +0.01% all damage reduction per CHA
                charmResist = 0.5,          -- +0.5% charm resistance per CHA
                threatReduction = 0.05      -- +0.05% less threat per CHA
            }
        },
        
        -- Armor-based reduction
        armorReduction = {
            formula = "damage * (1 - armor / (armor + 400 + 85 * attacker_level))",
            -- Standard armor formula: diminishing returns
            
            armorSources = {
                baseArmor = "From equipment",
                bonusArmor = "From stats and buffs",
                temporaryArmor = "From abilities"
            },
            
            -- Stats that grant armor
            statToArmor = {
                STR = 0.5,          -- +0.5 armor per STR
                CON = 0.3,          -- +0.3 armor per CON
                DEX = 0.2           -- +0.2 armor per DEX
            }
        },
        
        -- Magic resistance reduction
        magicResistance = {
            formula = "damage * (1 - resist / (resist + 350 + 75 * attacker_level))",
            
            -- Stats that grant magic resist
            statToResist = {
                INT = 0.6,          -- +0.6 magic resist per INT
                WIS = 0.5,          -- +0.5 magic resist per WIS
                CHA = 0.3           -- +0.3 magic resist per CHA
            }
        }
    },
    
    -- ========================================
    -- CLASS-SPECIFIC SCALING
    -- ========================================
    classScaling = {
        WIZARD = {
            primaryStat = "INT",
            damageDealt = {
                INT = {spellDamage = 0.05},     -- Wizards get 5% per INT (higher than base)
                WIS = {spellDamage = 0.01}      -- Secondary benefit from WIS
            },
            damageTaken = {
                INT = {spellReduction = 0.04},
                CON = {physicalReduction = 0.05},
                DEX = {dodgeChance = 0.05}      -- Light dodge capability
            },
            defensiveMechanics = {
                canBlock = false,
                canParry = false,
                canDodge = true
            }
        },
        
        SORCERER = {
            primaryStat = "CHA",
            damageDealt = {
                CHA = {spellDamage = 0.05},
                INT = {spellDamage = 0.01}
            },
            damageTaken = {
                CHA = {allReduction = 0.02},
                CON = {physicalReduction = 0.05},
                DEX = {dodgeChance = 0.06}
            },
            defensiveMechanics = {
                canBlock = false,
                canParry = false,
                canDodge = true
            }
        },
        
        PRIEST = {
            primaryStat = "WIS",
            damageDealt = {
                WIS = {spellDamage = 0.04, healingPower = 0.06},
                INT = {spellDamage = 0.01}
            },
            damageTaken = {
                WIS = {spellReduction = 0.05, healingReceived = 0.03},
                CON = {physicalReduction = 0.05},
                STR = {blockChance = 0.08}  -- Can block with shield
            },
            defensiveMechanics = {
                canBlock = true,        -- Requires shield
                canParry = false,
                canDodge = false
            }
        },
        
        FIGHTER = {
            primaryStat = "STR",
            damageDealt = {
                STR = {physicalDamage = 0.04},      -- 4% per STR
                DEX = {physicalDamage = 0.01}
            },
            damageTaken = {
                STR = {physicalReduction = 0.03, blockChance = 0.1},  -- +0.1% block with shield
                CON = {physicalReduction = 0.07},    -- Fighters benefit heavily from CON
                DEX = {dodgeChance = 0.08, parryChance = 0.05}  -- Dodge and parry
            },
            defensiveMechanics = {
                canBlock = true,        -- Requires shield
                canParry = true,        -- Can parry melee attacks
                canDodge = true         -- Can dodge attacks
            }
        },
        
        RANGER = {
            primaryStat = "DEX",
            damageDealt = {
                DEX = {physicalDamage = 0.03, rangedDamage = 0.05},
                STR = {physicalDamage = 0.01}
            },
            damageTaken = {
                DEX = {dodgeChance = 0.12, parryChance = 0.08},  -- Rangers dodge and parry
                CON = {physicalReduction = 0.05}
            },
            defensiveMechanics = {
                canBlock = false,
                canParry = true,
                canDodge = true
            }
        },
        
        WARLOCK = {
            primaryStat = "CHA",
            damageDealt = {
                CHA = {spellDamage = 0.045, petDamage = 0.05},
                INT = {spellDamage = 0.015}
            },
            damageTaken = {
                CHA = {allReduction = 0.015},
                CON = {physicalReduction = 0.05},
                DEX = {dodgeChance = 0.05}
            },
            defensiveMechanics = {
                canBlock = false,
                canParry = false,
                canDodge = true
            }
        },
        
        DRUID = {
            primaryStat = "WIS",
            damageDealt = {
                WIS = {spellDamage = 0.04, healingPower = 0.05},
                STR = {shapeshiftDamage = 0.02}      -- Benefits shapeshifted forms
            },
            damageTaken = {
                WIS = {spellReduction = 0.04},
                CON = {physicalReduction = 0.06},     -- Tankier in forms
                STR = {physicalReduction = 0.01},     -- When shapeshifted
                DEX = {dodgeChance = 0.08}            -- Agile in cat form
            },
            defensiveMechanics = {
                canBlock = false,
                canParry = false,
                canDodge = true     -- Especially in cat/travel forms
            }
        },
        
        MASTERMIND = {
            primaryStat = "CHA",
            damageDealt = {
                CHA = {petDamage = 0.08},            -- HUGE pet damage scaling
                INT = {spellDamage = 0.02}
            },
            damageTaken = {
                CHA = {allReduction = 0.02},         -- Personal protection
                CON = {physicalReduction = 0.05},
                DEX = {dodgeChance = 0.06},
                petDamageReduction = {               -- Pets take damage for master
                    CHA = 0.01                       -- +1% pet tanking per CHA
                }
            },
            defensiveMechanics = {
                canBlock = false,
                canParry = false,
                canDodge = true
            }
        },
        
        NECROMANCER = {
            primaryStat = "INT",
            damageDealt = {
                INT = {spellDamage = 0.045, petDamage = 0.04},
                WIS = {spellDamage = 0.01}
            },
            damageTaken = {
                INT = {spellReduction = 0.04},
                CON = {physicalReduction = 0.05},
                DEX = {dodgeChance = 0.05},
                lifeSteal = {INT = 0.05}             -- +0.05% lifesteal per INT
            },
            defensiveMechanics = {
                canBlock = false,
                canParry = false,
                canDodge = true
            }
        },
        
        PALADIN = {
            primaryStat = "STR",
            secondaryStat = "WIS",
            damageDealt = {
                STR = {physicalDamage = 0.035, holyDamage = 0.02},
                WIS = {healingPower = 0.04, holyDamage = 0.015}
            },
            damageTaken = {
                STR = {physicalReduction = 0.025, blockChance = 0.12},  -- Excellent blocker with shield
                CON = {physicalReduction = 0.06},
                WIS = {spellReduction = 0.03, healingReceived = 0.025}
            },
            defensiveMechanics = {
                canBlock = true,        -- Requires shield, best blocker
                canParry = true,
                canDodge = false
            }
        }
    },
    
    -- ========================================
    -- MONSTER SCALING
    -- ========================================
    monsterScaling = {
        -- Base monster stats
        baseStats = {
            health = 100,
            damage = 10,
            armor = 5,
            magicResist = 5
        },
        
        -- Level-based scaling
        levelScaling = {
            health = 1.15,          -- +15% HP per level
            damage = 1.10,          -- +10% damage per level
            armor = 1.08,           -- +8% armor per level
            magicResist = 1.08      -- +8% magic resist per level
        },
        
        -- Difficulty multipliers
        difficultyScaling = {
            Normal = {
                healthMultiplier = 1.0,
                damageMultiplier = 1.0,
                xpMultiplier = 1.0
            },
            Hard = {
                healthMultiplier = 1.5,
                damageMultiplier = 1.3,
                xpMultiplier = 1.5
            },
            Elite = {
                healthMultiplier = 2.5,
                damageMultiplier = 1.6,
                xpMultiplier = 3.0
            },
            Boss = {
                healthMultiplier = 5.0,
                damageMultiplier = 2.0,
                xpMultiplier = 10.0
            },
            RaidBoss = {
                healthMultiplier = 15.0,
                damageMultiplier = 2.5,
                xpMultiplier = 50.0
            }
        },
        
        -- Monster type scaling
        monsterTypeScaling = {
            Beast = {
                physicalDamageMultiplier = 1.2,
                armor = 1.1
            },
            Undead = {
                healthMultiplier = 0.8,
                damageMultiplier = 1.1,
                physicalResist = 0.3,  -- 30% physical resist
                holyVulnerability = 1.5  -- 50% more damage from holy
            },
            Demon = {
                spellDamageMultiplier = 1.3,
                magicResist = 1.2,
                holyVulnerability = 1.3
            },
            Elemental = {
                spellDamageMultiplier = 1.5,
                physicalResist = 0.5,  -- 50% physical resist
                elementalImmunity = true
            },
            Dragon = {
                healthMultiplier = 2.0,
                damageMultiplier = 1.5,
                armor = 1.5,
                magicResist = 1.5,
                breathWeapon = true
            },
            Humanoid = {
                healthMultiplier = 1.0,
                damageMultiplier = 1.0,
                canUseAbilities = true
            }
        },
        
        -- Individual monster overrides
        monsterOverrides = {
            -- Example: Lich King has custom scaling
            LICH_KING = {
                healthMultiplier = 20.0,
                damageMultiplier = 3.0,
                spellPower = 500,
                armor = 200,
                magicResist = 150
            },
            ANCIENT_DRAKE = {
                healthMultiplier = 10.0,
                damageMultiplier = 2.5,
                fireResist = 0.8,  -- 80% fire resist
                frostVulnerability = 1.5
            }
        }
    },
    
    -- ========================================
    -- DAMAGE CALCULATION FUNCTIONS
    -- ========================================
    calculateDamageDealt = function(self, attacker, baseDamage, damageType)
        local damage = baseDamage
        local classScaling = self.classScaling[attacker.class]
        
        if not classScaling then
            return damage
        end
        
        -- Apply primary stat scaling
        for stat, bonuses in pairs(classScaling.damageDealt) do
            local statValue = attacker.stats[stat] or 0
            
            if damageType == "physical" and bonuses.physicalDamage then
                damage = damage * (1 + statValue * bonuses.physicalDamage)
            elseif damageType == "spell" and bonuses.spellDamage then
                damage = damage * (1 + statValue * bonuses.spellDamage)
            elseif damageType == "ranged" and bonuses.rangedDamage then
                damage = damage * (1 + statValue * bonuses.rangedDamage)
            elseif damageType == "pet" and bonuses.petDamage then
                damage = damage * (1 + statValue * bonuses.petDamage)
            end
        end
        
        return damage
    end,
    
    calculateDamageTaken = function(self, defender, incomingDamage, damageType)
        local damage = incomingDamage
        local classScaling = self.classScaling[defender.class]
        
        if not classScaling then
            return damage
        end
        
        local totalReduction = 0
        
        -- Apply defensive stat scaling
        for stat, bonuses in pairs(classScaling.damageTaken) do
            local statValue = defender.stats[stat] or 0
            
            if damageType == "physical" and bonuses.physicalReduction then
                totalReduction = totalReduction + (statValue * bonuses.physicalReduction)
            elseif damageType == "spell" and bonuses.spellReduction then
                totalReduction = totalReduction + (statValue * bonuses.spellReduction)
            elseif bonuses.allReduction then
                totalReduction = totalReduction + (statValue * bonuses.allReduction)
            end
        end
        
        -- Cap damage reduction at 75%
        totalReduction = math.min(totalReduction, 0.75)
        
        -- Apply armor/resistance
        if damageType == "physical" then
            local armor = defender.armor or 0
            -- Add bonus armor from stats
            armor = armor + (defender.stats.STR or 0) * 0.5
            armor = armor + (defender.stats.CON or 0) * 0.3
            
            local armorReduction = armor / (armor + 400 + 85 * (defender.level or 1))
            totalReduction = totalReduction + armorReduction
        elseif damageType == "spell" then
            local resist = defender.magicResist or 0
            -- Add bonus resist from stats
            resist = resist + (defender.stats.INT or 0) * 0.6
            resist = resist + (defender.stats.WIS or 0) * 0.5
            
            local resistReduction = resist / (resist + 350 + 75 * (defender.level or 1))
            totalReduction = totalReduction + resistReduction
        end
        
        -- Cap total reduction at 90%
        totalReduction = math.min(totalReduction, 0.90)
        
        damage = damage * (1 - totalReduction)
        
        return damage
    end,
    
    -- ========================================
    -- EXAMPLE USAGE
    -- ========================================
    exampleUsage = {
        description = "How to use the stat-based damage system",
        
        example1 = [[
            -- Fighter with 50 STR deals physical damage
            local fighter = {class = "FIGHTER", level = 20, stats = {STR = 50, DEX = 20, CON = 40}}
            local baseDamage = 100
            local actualDamage = statSystem.calculateDamageDealt(statSystem, fighter, baseDamage, "physical")
            -- Result: ~220 damage (100 base * (1 + 50 * 0.04) = 100 * 3.0 = 300)
        ]],
        
        example2 = [[
            -- Wizard with 60 INT takes spell damage
            local wizard = {class = "WIZARD", level = 20, stats = {INT = 60, WIS = 30, CON = 20}, armor = 50, magicResist = 100}
            local incomingDamage = 200
            local actualDamage = statSystem.calculateDamageTaken(statSystem, wizard, incomingDamage, "spell")
            -- Result: Heavily reduced (multiple layers of reduction)
        ]],
        
        example3 = [[
            -- Mastermind with 70 CHA commanding pets
            local mastermind = {class = "MASTERMIND", level = 20, stats = {CHA = 70}}
            local petBaseDamage = 150
            local actualPetDamage = statSystem.calculateDamageDealt(statSystem, mastermind, petBaseDamage, "pet")
            -- Result: ~1050 damage (150 * (1 + 70 * 0.08) = 150 * 6.6 = 990)
        ]]
    }
}
