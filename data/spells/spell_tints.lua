-- Spell Visual Effects: Tints and Colors
-- Customizable color tints for all spells that can be overridden

return {
    -- ========================================
    -- DEFAULT TINTS BY SPELL SCHOOL
    -- ========================================
    schoolTints = {
        ARCANE = {
            primary = {r = 138, g = 43, b = 226},   -- Blue-Violet
            secondary = {r = 75, g = 0, b = 130},    -- Indigo
            particle = {r = 218, g = 112, b = 214},  -- Orchid
            glow = {r = 186, g = 85, b = 211, a = 0.7}
        },
        FIRE = {
            primary = {r = 255, g = 69, b = 0},      -- Red-Orange
            secondary = {r = 255, g = 140, b = 0},   -- Dark Orange
            particle = {r = 255, g = 215, b = 0},    -- Gold
            glow = {r = 255, g = 99, b = 71, a = 0.8}
        },
        FROST = {
            primary = {r = 0, g = 191, b = 255},     -- Deep Sky Blue
            secondary = {r = 135, g = 206, b = 250}, -- Light Sky Blue
            particle = {r = 240, g = 248, b = 255},  -- Alice Blue
            glow = {r = 173, g = 216, b = 230, a = 0.6}
        },
        SHADOW = {
            primary = {r = 75, g = 0, b = 130},      -- Indigo
            secondary = {r = 25, g = 25, b = 112},   -- Midnight Blue
            particle = {r = 138, g = 43, b = 226},   -- Blue Violet
            glow = {r = 72, g = 61, b = 139, a = 0.7}
        },
        HOLY = {
            primary = {r = 255, g = 215, b = 0},     -- Gold
            secondary = {r = 255, g = 255, b = 224}, -- Light Yellow
            particle = {r = 255, g = 250, b = 205},  -- Lemon Chiffon
            glow = {r = 255, g = 255, b = 0, a = 0.8}
        },
        NATURE = {
            primary = {r = 34, g = 139, b = 34},     -- Forest Green
            secondary = {r = 144, g = 238, b = 144}, -- Light Green
            particle = {r = 50, g = 205, b = 50},    -- Lime Green
            glow = {r = 0, g = 255, b = 127, a = 0.6}
        },
        PHYSICAL = {
            primary = {r = 192, g = 192, b = 192},   -- Silver
            secondary = {r = 169, g = 169, b = 169}, -- Dark Gray
            particle = {r = 211, g = 211, b = 211},  -- Light Gray
            glow = {r = 255, g = 255, b = 255, a = 0.4}
        },
        NECROMANCY = {
            primary = {r = 0, g = 100, b = 0},       -- Dark Green
            secondary = {r = 107, g = 142, b = 35},  -- Olive Drab
            particle = {r = 128, g = 128, b = 0},    -- Olive
            glow = {r = 154, g = 205, b = 50, a = 0.7}
        }
    },
    
    -- ========================================
    -- INDIVIDUAL SPELL TINT OVERRIDES
    -- ========================================
    spellTints = {
        -- Player Spells
        ["Fireball"] = {
            primary = {r = 255, g = 69, b = 0},
            trail = {r = 255, g = 140, b = 0},
            explosion = {r = 255, g = 215, b = 0},
            glow = {r = 255, g = 69, b = 0, a = 0.9}
        },
        
        ["Frost Bolt"] = {
            primary = {r = 135, g = 206, b = 250},
            trail = {r = 173, g = 216, b = 230},
            impact = {r = 255, g = 255, b = 255},
            glow = {r = 0, g = 191, b = 255, a = 0.7}
        },
        
        ["Holy Light"] = {
            primary = {r = 255, g = 255, b = 224},
            beam = {r = 255, g = 250, b = 205},
            sparkle = {r = 255, g = 215, b = 0},
            glow = {r = 255, g = 255, b = 0, a = 0.9}
        },
        
        ["Shadow Bolt"] = {
            primary = {r = 75, g = 0, b = 130},
            trail = {r = 106, g = 90, b = 205},
            impact = {r = 138, g = 43, b = 226},
            glow = {r = 72, g = 61, b = 139, a = 0.8}
        },
        
        ["Death Bolt"] = {
            primary = {r = 0, g = 100, b = 0},
            trail = {r = 85, g = 107, b = 47},
            impact = {r = 128, g = 128, b = 0},
            glow = {r = 107, g = 142, b = 35, a = 0.8},
            particles = {r = 154, g = 205, b = 50}
        },
        
        ["Divine Shield"] = {
            barrier = {r = 255, g = 215, b = 0},
            shimmer = {r = 255, g = 255, b = 224},
            pulse = {r = 255, g = 250, b = 205},
            glow = {r = 255, g = 255, b = 0, a = 0.6}
        },
        
        ["Consecration"] = {
            ground = {r = 255, g = 215, b = 0},
            rays = {r = 255, g = 255, b = 224},
            aura = {r = 255, g = 250, b = 205},
            glow = {r = 255, g = 255, b = 0, a = 0.5}
        },
        
        -- Mastermind Pet Enhancement Spells (NEW)
        ["Upgrade Pet"] = {
            primary = {r = 255, g = 140, b = 0},      -- Dark Orange
            secondary = {r = 255, g = 215, b = 0},    -- Gold
            particle = {r = 255, g = 255, b = 0},     -- Yellow
            spiral = {r = 255, g = 165, b = 0},       -- Orange
            glow = {r = 255, g = 140, b = 0, a = 0.9},
            runeColor = {r = 218, g = 165, b = 32}    -- Goldenrod
        },
        
        ["Enhance Pets"] = {
            primary = {r = 138, g = 43, b = 226},     -- Blue Violet
            secondary = {r = 147, g = 112, b = 219},  -- Medium Purple
            aura = {r = 186, g = 85, b = 211},        -- Medium Orchid
            particle = {r = 218, g = 112, b = 214},   -- Orchid
            glow = {r = 138, g = 43, b = 226, a = 0.8},
            buff = {r = 255, g = 0, b = 255}          -- Magenta
        },
        
        -- Pet Spells
        ["Dive Bomb"] = {
            primary = {r = 139, g = 69, b = 19},      -- Saddle Brown
            trail = {r = 210, g = 180, b = 140},      -- Tan
            impact = {r = 160, g = 82, b = 45},       -- Sienna
            glow = {r = 205, g = 133, b = 63, a = 0.6}
        },
        
        ["Shadow Bite"] = {
            primary = {r = 75, g = 0, b = 130},
            teeth = {r = 255, g = 255, b = 255},
            aura = {r = 106, g = 90, b = 205},
            glow = {r = 72, g = 61, b = 139, a = 0.7}
        },
        
        ["Bone Strike"] = {
            primary = {r = 245, g = 245, b = 220},    -- Beige
            secondary = {r = 211, g = 211, b = 211},  -- Light Gray
            impact = {r = 255, g = 255, b = 255},
            glow = {r = 240, g = 255, b = 240, a = 0.5}
        },
        
        ["Firebolt"] = {
            primary = {r = 255, g = 140, b = 0},
            trail = {r = 255, g = 69, b = 0},
            impact = {r = 255, g = 215, b = 0},
            glow = {r = 255, g = 99, b = 71, a = 0.8}
        }
    },
    
    -- ========================================
    -- CLASS-SPECIFIC TINT THEMES
    -- ========================================
    classTints = {
        WIZARD = {
            theme = "ARCANE",
            accentColor = {r = 138, g = 43, b = 226}
        },
        SORCERER = {
            theme = "FIRE",
            accentColor = {r = 255, g = 69, b = 0}
        },
        PRIEST = {
            theme = "HOLY",
            accentColor = {r = 255, g = 215, b = 0}
        },
        FIGHTER = {
            theme = "PHYSICAL",
            accentColor = {r = 192, g = 192, b = 192}
        },
        RANGER = {
            theme = "NATURE",
            accentColor = {r = 34, g = 139, b = 34}
        },
        WARLOCK = {
            theme = "SHADOW",
            accentColor = {r = 75, g = 0, b = 130}
        },
        DRUID = {
            theme = "NATURE",
            accentColor = {r = 34, g = 139, b = 34}
        },
        MASTERMIND = {
            theme = "ARCANE",
            accentColor = {r = 255, g = 140, b = 0},    -- Orange for control theme
            petEnhanceColor = {r = 138, g = 43, b = 226} -- Purple for buffs
        },
        NECROMANCER = {
            theme = "NECROMANCY",
            accentColor = {r = 0, g = 100, b = 0}
        },
        PALADIN = {
            theme = "HOLY",
            accentColor = {r = 255, g = 215, b = 0}
        }
    },
    
    -- ========================================
    -- SPECIAL EFFECT TINTS
    -- ========================================
    specialEffects = {
        CRITICAL_HIT = {
            flash = {r = 255, g = 255, b = 0},
            spark = {r = 255, g = 215, b = 0},
            glow = {r = 255, g = 255, b = 0, a = 1.0}
        },
        HEAL = {
            pulse = {r = 0, g = 255, b = 127},
            shimmer = {r = 144, g = 238, b = 144},
            glow = {r = 50, g = 205, b = 50, a = 0.7}
        },
        BUFF = {
            aura = {r = 173, g = 216, b = 230},
            sparkle = {r = 135, g = 206, b = 250},
            glow = {r = 0, g = 191, b = 255, a = 0.5}
        },
        DEBUFF = {
            aura = {r = 139, g = 0, b = 0},
            cloud = {r = 178, g = 34, b = 34},
            glow = {r = 220, g = 20, b = 60, a = 0.6}
        },
        PET_UPGRADE = {
            spiral = {r = 255, g = 215, b = 0},
            burst = {r = 255, g = 255, b = 0},
            shimmer = {r = 255, g = 250, b = 205},
            rune = {r = 218, g = 165, b = 32},
            glow = {r = 255, g = 215, b = 0, a = 0.9}
        },
        PET_ENHANCE = {
            wave = {r = 138, g = 43, b = 226},
            aura = {r = 186, g = 85, b = 211},
            particle = {r = 218, g = 112, b = 214},
            connection = {r = 147, g = 112, b = 219},
            glow = {r = 138, g = 43, b = 226, a = 0.8}
        }
    },
    
    -- ========================================
    -- UTILITY FUNCTIONS
    -- ========================================
    getTint = function(self, spellName, element)
        -- Check for spell-specific override first
        if self.spellTints[spellName] then
            return self.spellTints[spellName][element] or self.spellTints[spellName].primary
        end
        
        -- Fall back to school tint
        -- (Would need to look up spell school from spell data)
        return nil
    end,
    
    getClassTint = function(self, className)
        if self.classTints[className] then
            local theme = self.classTints[className].theme
            return self.schoolTints[theme]
        end
        return nil
    end,
    
    blendTints = function(self, tint1, tint2, ratio)
        -- Blend two tints together (ratio 0.0 to 1.0)
        return {
            r = math.floor(tint1.r * (1 - ratio) + tint2.r * ratio),
            g = math.floor(tint1.g * (1 - ratio) + tint2.g * ratio),
            b = math.floor(tint1.b * (1 - ratio) + tint2.b * ratio),
            a = (tint1.a or 1.0) * (1 - ratio) + (tint2.a or 1.0) * ratio
        }
    end
}
