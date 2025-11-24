# CloneMine Core Game Data

This directory contains the core game data for CloneMine, organized in a fully modular structure.

## Directory Structure

```
data/
├── classes/              # All 9 character classes (fully modularized)
│   ├── wizard/          # Standard 4-file structure
│   │   ├── wizard_stats.lua
│   │   ├── wizard_spells.lua
│   │   ├── wizard_abilities.lua
│   │   └── wizard_feats.lua
│   ├── sorcerer/
│   ├── priest/
│   ├── fighter/
│   ├── ranger/          # Pet class - 6 files
│   │   ├── ranger_stats.lua
│   │   ├── ranger_spells.lua
│   │   ├── ranger_abilities.lua
│   │   ├── ranger_feats.lua
│   │   ├── ranger_summoning.lua    # Pet mechanics
│   │   └── ranger_pets.lua         # Pet definitions
│   ├── warlock/         # Pet class
│   ├── druid/           # Shapeshifter
│   ├── mastermind/      # Pet controller (6 pets)
│   └── necromancer/     # Death mage (5 pets)
├── spells/
│   ├── player_spells.lua
│   └── monster_spells.lua
├── abilities/
│   ├── player_abilities.lua
│   └── monster_abilities.lua
├── equipment/
│   ├── equipment_system.lua
│   ├── armor.lua
│   └── weapons.lua
├── items/
│   └── items.lua
├── monsters/
│   └── monsters_quests.lua
├── encounters/
│   └── encounter_tables.lua
├── class_loader.lua     # Dynamic class loading
└── content_loader.lua   # Master loader
```

## Character Classes (Fully Modularized)

All 9 character classes are broken down into modular files. NO classes remain in a single monolithic file.

### Standard Class Files (4 files)

Every class has these 4 required files:

1. **`{class}_stats.lua`** - Base statistics
   - Primary stat, resource type (Mana/Energy/Focus/Control)
   - Health, resource pools, regeneration rates
   - Equipment restrictions (weapons, armor)
   - Starting stats and stat growth

2. **`{class}_spells.lua`** - Spell list
   - All spells available to the class
   - Spell unlock levels

3. **`{class}_abilities.lua`** - Active abilities
   - Class-specific abilities with cooldowns and costs
   - NOT including pet-related abilities (those are in summoning)
   - Unlock levels for each ability

4. **`{class}_feats.lua`** - Passive bonuses (D&D style)
   - **Earned every 2 levels**: 2, 4, 6, 8, 10, 12, 14, 16, 18, 20
   - Total of 10 feats by level 20
   - Player chooses 1 feat per feat level
   - NOT earned per kill - earned automatically at specific levels

### Pet Class Additional Files (2 files)

Classes with pets have 2 additional files for pet systems:

5. **`{class}_summoning.lua`** - Summoning mechanics
   - Max summons allowed
   - Tier system (tier 1, 2, 3)
   - Limits per tier
   - Unlock levels
   - Summoning rules

6. **`{class}_pets.lua`** - Pet definitions (SEPARATE from summoning)
   - Complete stat blocks for each pet type
   - Pet abilities (not player abilities)
   - Behaviors, AI, special properties
   - Scaling formulas

### The 9 Classes

| Class | Primary Stat | Resource | Pets | Files |
|-------|--------------|----------|------|-------|
| **Wizard** | INT | Mana | No | 4 |
| **Sorcerer** | CHA | Mana | No | 4 |
| **Priest** | WIS | Mana | No | 4 |
| **Fighter** | STR | Energy | No | 4 |
| **Ranger** | DEX | Focus | 1 | 6 |
| **Warlock** | CHA | Mana | 1 | 6 |
| **Druid** | WIS | Mana | No* | 4 |
| **Mastermind** | CHA | Control | 6 | 6 |
| **Necromancer** | INT | Mana | 5 | 6 |

\* Druid has shapeshifting instead of pets (in separate shapeshift_forms.lua)

## Feat System (D&D 3.5 / Pathfinder Style)

Feats are earned **every 2 levels**, not per kill or action:

### Feat Progression
- **Feat Levels**: 2, 4, 6, 8, 10, 12, 14, 16, 18, 20
- **Total Feats**: 10 by level 20
- **Selection**: Player chooses 1 feat from available options at each feat level
- **Automatic**: Feats unlock at specific levels, similar to D&D

### Feat Tiers

Each class has 10+ feat options organized by tier:

- **Early Feats** (Levels 2-6): Basic improvements
  - Example: +5% damage, +10% mana, +5% crit

- **Mid Feats** (Levels 8-14): Specialized abilities
  - Example: Reduced cooldowns, improved abilities, unique mechanics

- **Advanced Feats** (Levels 16-20): Powerful capstones
  - Example: Ultimate power boosts, game-changing passives

## Pet System Architecture

Pet classes separate mechanics from definitions:

### Why Separate Files?

**Summoning File** (`{class}_summoning.lua`):
- Defines HOW MANY pets
- Tier limits (e.g., 3 tier 1, 2 tier 2, 1 tier 3)
- Unlock levels
- Summoning mechanics

**Pets File** (`{class}_pets.lua`):
- Defines WHAT the pets are
- Complete stat blocks
- Pet-specific abilities (not player abilities)
- Behaviors and AI
- Scaling formulas

### Benefits of Separation

1. **Balance**: Adjust numbers vs stats independently
2. **Clarity**: Pet logic separate from player abilities
3. **Maintainability**: Easy to find and modify pet data
4. **Extensibility**: Plugins can add new pets without touching mechanics

### Pet Class Examples

**Ranger** (1 pet):
- Summoning: 1 animal companion, choose at level 1
- Pets: Wolf, Bear, or Hawk with full stat blocks

**Mastermind** (6 pets):
- Summoning: 3 Minions + 2 Lieutenants + 1 Boss Pet
- Pets: Henchman, Elite Enforcer, Champion definitions

**Necromancer** (5 pets):
- Summoning: 3 Skeletons + 1 Zombie + 1 Ghoul
- Pets: Undead minion stat blocks with special properties

## Dynamic Loading System

The `class_loader.lua` module:

### Features
- Automatically discovers all class directories
- Loads all required files (stats, spells, abilities, feats)
- Loads optional files (summoning, pets) for pet classes
- Validates file existence
- Provides clean API for accessing class data
- Supports feat progression queries

### API Examples

```lua
local ClassLoader = require("data/class_loader")

-- Initialize and load all classes
ClassLoader:init()
ClassLoader:loadAllClasses()

-- Get class data
local wizardStats = ClassLoader:getClassStats("WIZARD")
local necromancerPets = ClassLoader:getClassPets("NECROMANCER")

-- Feat progression
local featInfo = ClassLoader:getFeatProgression()
-- Returns: {levels = {2,4,6,8,10,12,14,16,18,20}, totalFeats = 10, ...}

-- Check pet classes
if ClassLoader:canClassSummon("RANGER") then
    local maxPets = ClassLoader:getMaxSummons("RANGER")  -- Returns 1
end
```

## Plugin Extension System

Plugins in `plugins/` directory can extend core classes:

### What Plugins Can Do

1. **Add spells to existing classes**
   ```lua
   ClassLoader:addSpellToClass("WIZARD", {
       name = "Meteor Swarm",
       ...
   })
   ```

2. **Add abilities to classes**
   ```lua
   ClassLoader:addAbilityToClass("FIGHTER", {
       name = "Titan's Grip",
       ...
   })
   ```

3. **Add new shapeshift forms** (Druid only)

4. **Create equipment set bonuses**

5. **Register entirely new classes**
   ```lua
   ClassLoader:registerClass("PALADIN", "plugins/paladin")
   ```

See `plugins/README.md` for detailed extension examples.

## Design Principles

1. **Fully Modular**: EVERY class is broken down, no monolithic files
2. **Consistent Structure**: All classes follow same pattern
3. **Separation of Concerns**: Pet logic separate from abilities
4. **D&D-Inspired**: Feat progression matches D&D 3.5/Pathfinder
5. **Extensible**: Plugins can modify without editing core
6. **Maintainable**: Easy to find and modify any class data
7. **Balanced**: Independent tuning of different aspects

## File Naming Convention

All files follow strict naming:
- `{classname}_stats.lua`
- `{classname}_spells.lua`
- `{classname}_abilities.lua`
- `{classname}_feats.lua`
- `{classname}_summoning.lua` (pet classes only)
- `{classname}_pets.lua` (pet classes only)

Where `{classname}` is lowercase (wizard, necromancer, etc.)

## Core vs Plugins

| Directory | Purpose | Modifiable |
|-----------|---------|------------|
| `data/` | Core game content | Not by plugins (but extendable) |
| `plugins/` | Extensions and custom content | Yes (user-created) |

Core data is loaded first, then plugins extend it.
