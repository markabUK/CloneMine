# CloneMine Plugin System

This directory contains **extension plugins** that demonstrate how to add custom content to the core game.

## Important: Core vs Plugin Content

### Core Game Data (`data/` directory)
The main game content is located in the `data/` directory:
- `data/classes/` - 9 character classes (Wizard, Sorcerer, Priest, Fighter, Ranger, Warlock, Druid, Mastermind, Necromancer)
- `data/spells/` - Player and monster spells
- `data/abilities/` - Player and monster abilities
- `data/equipment/` - Weapons, armor, jewelry systems
- `data/items/` - Core items and consumables
- `data/monsters/` - Monster definitions and quests
- `data/encounters/` - Random encounter tables

See `data/README.md` for complete core game documentation.

### Plugin Extensions (`plugins/` directory)
This directory contains **only extension plugins** that add custom content:
- New classes, spells, or abilities
- Custom monsters and bosses
- New items, weapons, and armor
- Custom blocks and world features
- Modifications to existing systems

---

## Example Extension Plugins

### 1. **example_class_extension.lua**
Shows how to extend existing classes with:
- Custom spells (e.g., "Arcane Meteor" for Wizard)
- New passive abilities (e.g., "Soul Reaper" for Necromancer)
- Additional shapeshift forms (e.g., "Phoenix" for Druid)
- Equipment set bonuses (e.g., "Berserker's Rage" for Fighter)

**Key Patterns:**
```lua
-- Add a custom spell to an existing class
function extendWizardClass()
    local customSpell = {
        name = "Arcane Meteor",
        school = "Arcane",
        level = 15,
        manaCost = 80,
        damage = 200,
        aoeRadius = 10
    }
    -- Register with game
end
```

### 2. **example_item_extension.lua**
Demonstrates adding:
- Custom consumables (Mega Potions, Stat Food, Elixirs)
- Legendary weapons (Frostmourne, Staff of Dominance)
- Epic armor sets (Shadow Assassin, Archmage)
- Legendary jewelry (The One Ring, Phoenix Amulet)

**Key Patterns:**
```lua
-- Add a custom legendary weapon
function addCustomWeapons()
    local weapon = {
        id = "FROSTMOURNE",
        name = "Frostmourne",
        type = "2H_SWORD",
        damage = 30,
        stats = {STR = 50, INT = 30},
        specialAbilities = {"Soul Reaper", "Frozen Soul"},
        rarity = "Legendary",
        restrictedTo = {"FIGHTER", "NECROMANCER"}
    }
end
```

### 3. **example_monster_extension.lua**
Shows how to add:
- Custom regular monsters (Frost Giant, Shadow Stalker, Corrupted Treant)
- Epic raid bosses (Tiamat/Dragon Queen, Xal'atath/Void Lord)
- World events (Demonic Invasion)
- Multi-phase boss encounters

**Key Patterns:**
```lua
-- Add a custom boss with phases
function addCustomBosses()
    local boss = {
        id = "DRAGON_QUEEN",
        name = "Tiamat, Dragon Queen",
        level = 20,
        health = 100000,
        phases = {
            {threshold = 100, abilities = {...}},
            {threshold = 75, abilities = {...}},
            {threshold = 50, abilities = {...}},
            {threshold = 25, abilities = {...}}
        },
        requiresGroup = true,
        minPlayers = 10
    }
end
```

### 4. **custom_blocks.lua**
Example of adding custom block types:
- Obsidian, Glass, Brick
- Ore blocks (Gold, Diamond, Emerald)
- Environmental blocks (Ice, Lava)
- Block properties (hardness, transparency, luminance)

### 5. **example.lua**
Basic plugin template showing:
- Plugin initialization
- Logging system
- Basic API usage

---

## Creating Your Own Plugin

### Basic Plugin Structure

```lua
-- My Custom Plugin

function onLoad()
    log("Loading My Custom Plugin")
    
    -- Initialize your custom content here
    addCustomContent()
    
    log("My Custom Plugin loaded successfully!")
end

function addCustomContent()
    -- Your custom items, spells, monsters, etc.
end

-- Call onLoad when plugin is loaded
onLoad()
```

### Plugin API Reference

#### Logging
```lua
log("Message")  -- Output to game log
```

#### Game State Access
```lua
-- Access player data
getPlayerLevel()
getPlayerClass()
getPlayerStats()

-- Access world data
getBlockType(x, y, z)
setBlockType(x, y, z, blockType)
```

#### Registering Content
```lua
-- Register custom content with the game
registerSpell(spellData)
registerAbility(abilityData)
registerMonster(monsterData)
registerItem(itemData)
```

---

## Plugin Guidelines

### DO:
✅ Add new content that extends the game
✅ Create unique items, spells, and abilities
✅ Add custom monsters and bosses
✅ Modify drop tables and loot
✅ Create new world features and blocks
✅ Add cosmetic features and effects

### DON'T:
❌ Modify core class definitions (extend instead)
❌ Remove or replace core game content
❌ Break game balance without consideration
❌ Access unsafe system functions (sandboxed)
❌ Override security restrictions

---

## Plugin Loading

Plugins are loaded automatically from this directory at game startup. The loading order is:
1. Core game data (`data/`)
2. Plugin extensions (`plugins/`)

Plugins can reference and extend core content but should not replace it.

---

## Advanced Topics

### Extending Classes
Plugins can add new spells, abilities, and features to existing classes without modifying core class files.

### Custom Equipment Sets
Create set bonuses that activate when multiple pieces are equipped together.

### World Events
Add timed events that spawn special monsters or create unique encounters.

### Boss Mechanics
Implement complex multi-phase boss encounters with special abilities and loot tables.

### Mod Compatibility
Well-designed plugins will be compatible with other plugins that don't overlap in functionality.

---

## Support and Resources

- Core game documentation: `data/README.md`
- Building instructions: `BUILDING.md`
- Security information: `SECURITY.md`
- Quick start guide: `QUICKSTART.md`

---

## Examples Summary

| Plugin File | Purpose | Key Features |
|------------|---------|--------------|
| `example_class_extension.lua` | Extend classes | Custom spells, abilities, forms, sets |
| `example_item_extension.lua` | Add items | Weapons, armor, consumables, jewelry |
| `example_monster_extension.lua` | Add monsters | Regular mobs, raid bosses, world events |
| `custom_blocks.lua` | Add blocks | New block types and properties |
| `example.lua` | Basic template | Simple plugin structure |

Start with these examples and modify them to create your own custom content!
