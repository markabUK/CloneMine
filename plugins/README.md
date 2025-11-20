# CloneMine Plugins

This directory contains example Lua plugins demonstrating the CloneMine plugin system.

## Available Plugins

### 1. example.lua
Basic plugin demonstrating the core plugin API hooks:
- `onLoad()` - Called when plugin loads
- `onUpdate(deltaTime)` - Called every frame
- `onInput()` - Handle input events
- `onRender()` - Called during rendering

### 2. custom_blocks.lua
Adds 8 new block types with properties:

**Blocks:**
- **Obsidian** - Very hard, high blast resistance
- **Gold Ore** - Drops gold nuggets
- **Diamond Ore** - Drops diamonds
- **Emerald Ore** - Drops emeralds
- **Glass** - Transparent block
- **Brick** - Durable building material
- **Ice** - Transparent, slippery surface
- **Lava** - Luminous, damaging liquid

**Features:**
- Block definitions with hardness, transparency, and special properties
- Ore generation functions
- Drop item system

### 3. items.lua
Complete item system with materials, tools, and consumables:

**Materials (8 items):**
- Stick, Coal, Iron Ingot, Gold Ingot
- Diamond, Emerald, Leather, String

**Tools (4 pickaxes):**
- Wooden, Stone, Iron, Diamond
- Each with durability and mining speed stats

**Consumables (4 items):**
- Apple, Bread, Cooked Meat, Golden Apple
- Healing and status effect properties

**Features:**
- Item categories and rarity system
- Stack size limits
- Durability tracking
- Consumable effects

### 4. armor.lua
Comprehensive armor system with protection values:

**Armor Materials (5 types):**
- Leather (Total Protection: 8)
- Chainmail (Total Protection: 12)
- Iron (Total Protection: 15)
- Gold (Total Protection: 11, high enchantability)
- Diamond (Total Protection: 20)

**Features:**
- Four armor slots: Helmet, Chestplate, Leggings, Boots
- Durability system for each piece
- Enchantment support
- Total protection calculation
- Armor damage and repair

**Enchantments:**
- Protection, Fire Protection, Blast Protection
- Projectile Protection, Feather Falling
- Thorns, Unbreaking

### 5. weapons.lua
Weapon system with damage stats and combat mechanics:

**Weapon Materials (5 types):**
- Wooden (Damage: 4, Durability: 59)
- Stone (Damage: 5, Durability: 131)
- Iron (Damage: 6, Durability: 250)
- Gold (Damage: 4, Durability: 32, high enchantability)
- Diamond (Damage: 7, Durability: 1561)

**Special Weapons:**
- **Excalibur** - Legendary sword with Divine Strike
- **Dragon Slayer** - Bonus damage against dragons

**Features:**
- Critical hit system (5% base chance, 1.5x damage)
- Attack speed mechanics
- Durability tracking
- Enchantment support
- Combat simulation

**Enchantments:**
- Sharpness, Smite, Bane of Arthropods
- Knockback, Fire Aspect, Looting
- Sweeping Edge, Unbreaking

### 6. player_character.lua
Complete player stats and progression system:

**Stats:**
- **Health**: 20 HP (regenerates with high hunger)
- **Hunger**: 20 points (depletes over time)
- **Stamina**: 100 points (for sprinting)
- **Movement**: Walk speed, sprint speed, jump height
- **Combat**: Base damage, attack range

**Attributes (10 points each):**
- **Strength** - Increases melee damage
- **Defense** - Reduces incoming damage
- **Agility** - Movement speed and dodge chance
- **Vitality** - Increases max health
- **Intelligence** - Magic and enchanting
- **Luck** - Drops and critical hits

**Status Effects:**
- Regeneration, Strength, Speed, Jump Boost
- Resistance, Fire Resistance, Water Breathing
- Invisibility, Poison, Weakness

**Features:**
- Level and experience system
- Attribute point distribution
- Status effect duration tracking
- Damage calculation with defense
- Hunger and health mechanics
- Player stat display

## Using the Plugins

### Basic Usage

All plugins are automatically loaded from the `plugins/` directory when CloneMine starts.

### Plugin API Functions

**Available to all plugins:**

```lua
-- Logging
log(message)

-- Block manipulation
getBlockType(x, y, z)
setBlockType(x, y, z, blockType)
```

### Plugin Hooks

All plugins can implement these callback functions:

```lua
function onLoad()
    -- Called once when plugin loads
    -- Use for initialization
end

function onUpdate(deltaTime)
    -- Called every frame
    -- deltaTime is time since last frame in seconds
end

function onInput()
    -- Called when input events occur
end

function onRender()
    -- Called during the rendering phase
end
```

## Creating Custom Plugins

1. Create a new `.lua` file in the `plugins/` directory
2. Implement the plugin hooks you need
3. Use only the safe API functions provided
4. Restart CloneMine to load your plugin

### Example Custom Plugin

```lua
-- my_custom_plugin.lua

local blockCounter = 0

function onLoad()
    log("My Custom Plugin loaded!")
    blockCounter = 0
end

function onUpdate(deltaTime)
    -- Update logic here
end

-- Custom functionality
function placeCustomBlock(x, y, z)
    setBlockType(x, y, z, 10)  -- Custom block type
    blockCounter = blockCounter + 1
    log("Placed block #" .. blockCounter)
end
```

## Security

The plugin system is **sandboxed** for security:

### ✅ Allowed
- Safe Lua standard library functions (math, string, table)
- Logging via `log()`
- Block manipulation via API
- Pure computation and game logic

### ❌ Blocked
- File I/O operations (`io.*`, `dofile`, `loadfile`)
- System commands (`os.execute`, `os.exit`)
- Code loading (`load`, `require`, `package.*`)
- Debug introspection (`debug.*`)
- Network access

## Plugin Development Tips

1. **Test Frequently**: Restart CloneMine after changes
2. **Use log()**: Debug with log messages to the console
3. **Handle Errors**: Lua errors won't crash the game
4. **Keep It Simple**: Complex plugins may impact performance
5. **Document Your Code**: Help others understand your plugins

## Example Scenarios

### Equipping Full Diamond Armor

```lua
-- In the Lua console or plugin
equipArmor(ArmorSlot.HELMET, armorSets["DIAMOND"].helmet)
equipArmor(ArmorSlot.CHESTPLATE, armorSets["DIAMOND"].chestplate)
equipArmor(ArmorSlot.LEGGINGS, armorSets["DIAMOND"].leggings)
equipArmor(ArmorSlot.BOOTS, armorSets["DIAMOND"].boots)
getTotalProtection()  -- Should return 20
```

### Creating an Enchanted Weapon

```lua
local sword = Weapon:new("DIAMOND", WeaponType.SWORD)
sword:addEnchantment("SHARPNESS", 5)
sword:addEnchantment("FIRE_ASPECT", 2)
equipWeapon(sword)
```

### Leveling Up the Player

```lua
addExperience(500)  -- Gain XP
increaseAttribute("strength", 5)  -- Increase strength
displayPlayerStats()  -- Show all stats
```

## Contributing

Feel free to create and share your own plugins! The plugin system is designed to be extensible and safe.

## License

These example plugins are provided as part of CloneMine and are free to use and modify.
