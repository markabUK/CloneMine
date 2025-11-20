# Core Game Data

This directory contains the core game data for CloneMine's RPG system. Unlike the `plugins/` directory, this data is part of the base game and loaded automatically at startup.

## Directory Structure

```
data/
├── classes/              # Character class definitions
│   ├── wizard/          # Each class has its own directory
│   │   ├── wizard_stats.lua
│   │   ├── wizard_spells.lua
│   │   ├── wizard_abilities.lua
│   │   ├── wizard_feats.lua
│   │   └── wizard_summoning.lua (if applicable)
│   ├── necromancer/
│   ├── ...
│   └── class_registry.lua
├── spells/              # Spell definitions
│   ├── player_spells.lua
│   └── monster_spells.lua
├── abilities/           # Ability definitions
│   ├── player_abilities.lua
│   └── monster_abilities.lua
├── monsters/            # Monster definitions
│   ├── monster_registry.lua
│   └── monster_ai.lua
├── equipment/           # Equipment definitions
│   ├── weapons.lua
│   ├── armor.lua
│   └── jewelry.lua
├── encounters/          # Random encounter system
│   └── encounter_tables.lua
├── class_loader.lua     # Dynamic class loading system
└── content_loader.lua   # Master content loader
```

## Character Classes

Each character class is modular and consists of up to 5 files:

### 1. `{class}_stats.lua`
Defines base stats, resources, equipment restrictions:
- Primary stat (STR, DEX, CON, INT, WIS, CHA)
- Resource type (Mana, Energy, Focus, etc.)
- Base health and resource pool
- Armor type restrictions (Cloth, Leather, Mail, Plate)
- Weapon type restrictions
- Level progression bonuses

### 2. `{class}_spells.lua`
Class-specific spells and shared spells:
- Spell list with level requirements
- Cast time, cooldown, cost
- Damage/healing values
- Special effects

### 3. `{class}_abilities.lua`
Class abilities (non-spell actions):
- Active abilities
- Resource costs
- Cooldowns
- Special mechanics

### 4. `{class}_feats.lua`
Passive bonuses and talents:
- Passive stat bonuses
- Talent trees
- Specialization options
- Level-based unlocks

### 5. `{class}_summoning.lua` (optional)
Pet and summon definitions for classes that have them:
- Pet types and stats
- Summon limits per tier
- Pet abilities
- Control mechanics

## Available Classes

1. **Wizard** (INT) - Arcane caster, high burst damage
2. **Sorcerer** (CHA) - Natural magic, sustained damage
3. **Priest** (WIS) - Holy healer and support
4. **Fighter** (STR) - Melee warrior, high survivability
5. **Ranger** (DEX) - Ranged DPS with pet
6. **Warlock** (CHA) - Shadow magic with demons
7. **Druid** (WIS) - Nature magic, shapeshifting
8. **Mastermind** (CHA) - Pet controller with 6 pets
9. **Necromancer** (INT) - Death magic with undead army

## Dynamic Loading

Classes are loaded dynamically at startup:

```lua
-- The class_loader.lua automatically discovers and loads all class files
local ClassLoader = require("data/class_loader")
ClassLoader:loadAllClasses()

-- Access loaded class data
local wizard = ClassLoader:getClass("WIZARD")
local necromancer = ClassLoader:getClass("NECROMANCER")
```

## Plugin Extension

Plugins can extend the core game data:

```lua
-- In a plugin, add custom spells to a class
local ClassLoader = require("data/class_loader")
ClassLoader:addSpellToClass("WIZARD", {
    name = "Custom Fireball",
    damage = 200,
    -- ... other properties
})

-- Or create entirely new classes
ClassLoader:registerClass("BATTLEMAGE", {
    statsFile = "path/to/battlemage_stats.lua",
    -- ... other files
})
```

## Random Encounters

The encounter system generates dynamic monster spawns based on:
- Player location (biome type)
- Time of day
- Player level (±3 levels)
- Rarity roll (Common to Epic)

```lua
local Encounters = require("data/encounters/encounter_tables")
local encounter = Encounters:rollEncounter("FOREST", "NIGHT", playerLevel)
-- Returns a scaled monster appropriate for the location and level
```

## Monster vs Player Content

Spells and abilities are separated into player and monster versions:
- **Player spells/abilities**: Balanced for player use, reasonable costs
- **Monster spells/abilities**: Designed for NPC use, boss mechanics

This separation allows for better game balance and unique monster behaviors without affecting player power levels.

## Leveling System

- **Current Cap**: Level 20
- **Future Cap**: Level 120 (planned)
- **Stat Points**: 3 per level
- **Ability Unlocks**: New abilities at specific levels (5, 10, 15, 20)

## Summoning Classes

### Mastermind (6 pets max)
- Tier 1: 3x Minions (basic)
- Tier 2: 2x Lieutenants (strong)
- Tier 3: 1x Boss Pet (powerful)

### Necromancer (5 pets max)
- Tier 1: 3x Skeletons (melee)
- Tier 2: 1x Zombie (tank)
- Tier 3: 1x Ghoul (elite)

### Ranger (1 pet)
- Single loyal companion with deep bond

### Warlock (2 pets max)
- Tier 1: 1x Imp (utility)
- Tier 2: 1x Demon (combat)

## File Format

All data files return Lua tables:

```lua
-- Example: necromancer_stats.lua
return {
    name = "Necromancer",
    primaryStat = "INT",
    resourceType = "Mana",
    baseHealth = 80,
    baseResource = 150,
    allowedArmor = {"CLOTH"},
    allowedWeapons = {"WAND", "STAFF", "DAGGER"},
    canDualWield = false,
    canUseShield = false
}
```

## Modification Guidelines

When modifying core data:
1. **Maintain balance**: Changes affect the entire game
2. **Test thoroughly**: Core data impacts all players
3. **Document changes**: Update this README if adding new systems
4. **Version compatibility**: Consider save game compatibility

For experimental features, use plugins instead of modifying core data.
