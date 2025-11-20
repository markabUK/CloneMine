# CloneMine Plugins - D&D-Style RPG System

This directory contains Lua plugins implementing a complete D&D-inspired RPG system.

## Core Systems

### 1. character_classes.lua
**D&D-Style Character Class System**

Implements 7 playable classes with unique mechanics:

**Classes:**
- **Wizard** - INT-based mana caster
  - Weapons: Staff, Wand, Dagger | Armor: Cloth
  - Abilities: Magic Missile, Fireball, Ice Storm, Lightning Bolt, Meteor

- **Sorcerer** - CHA-based mana caster  
  - Weapons: Staff, Wand, Dagger | Armor: Cloth
  - Abilities: Chaos Bolt, Wild Magic Surge, Arcane Barrage, Time Stop, Wish

- **Priest** - WIS-based mana healer
  - Weapons: Mace, Staff, Wand | Armor: Cloth, Leather, Mail | Shield: Yes
  - Abilities: Heal, Holy Smite, Divine Shield, Mass Heal, Resurrection

- **Fighter** - STR-based energy warrior
  - Weapons: All | Armor: All | Dual-Wield: Yes | Shield: Yes
  - Abilities: Power Strike, Whirlwind, Execute, Battle Cry, Bladestorm

- **Ranger** - DEX-based focus user with pet
  - Weapons: Bow, Crossbow, Sword, Dagger, Spear | Armor: Cloth, Leather, Mail | Dual-Wield: Yes
  - Abilities: Aimed Shot, Call Pet, Multi-Shot, Trap, Camouflage, Barrage

- **Warlock** - CHA-based soul energy with pet
  - Weapons: Staff, Wand, Dagger | Armor: Cloth, Leather
  - Abilities: Shadow Bolt, Summon Demon, Curse of Weakness, Drain Life, Fear, Soul Siphon

- **Druid** - WIS-based nature power shapeshifter
  - Weapons: Staff, Dagger, Mace | Armor: Cloth, Leather
  - Abilities: Wrath, Bear Form, Healing Touch, Moonfire, Cat Form, Starfall

**Features:**
- D&D stats: STR, DEX, CON, INT, WIS, CHA
- Resources regen faster out of combat (10/s) vs in combat (2/s)
- Level progression with XP system
- 3 stat points per level
- Abilities unlock at specific levels

### 2. equipment_system.lua
**Complete Equipment System**

**11 Equipment Slots:**
- Weapons: Main Hand, Off Hand
- Armor: Helmet, Shoulder, Chest, Belt, Bracer, Gloves, Legs, Boots
- Jewelry: Necklace, Ring 1, Ring 2

**Weapon Types:**
- 1H: Sword (8), Axe (9), Mace (7), Dagger (5), Wand (6+spell)
- 2H: Greatsword (15), Battleaxe (16), Staff (8+25spell), Bow (12), Crossbow (14), Spear (11)
- Shield: +5 armor (off-hand, 1H weapons only)

**Armor Types & Class Restrictions:**
- Cloth: All casters
- Leather: Ranger, Druid, Priest
- Mail: Fighter, Priest, Ranger
- Plate: Fighter only

**Mechanics:**
- Dual-wielding (Fighter, Ranger)
- Shield use (Fighter, Priest with 1H weapon)
- 2H weapons prevent off-hand
- Class-appropriate weapon restrictions

## Usage Examples

### Complete RPG Setup

```lua
-- Choose class
setPlayerClass("WIZARD")

-- Equip gear
equipWeapon(ExampleWeapons.apprenticeWand, EquipmentSlot.MAIN_HAND, PlayerCharacter.class)
equipArmor(ExampleArmor.robesOfPower, PlayerCharacter.class)
equipJewelry(ExampleJewelry.necklace1, EquipmentSlot.NECKLACE)

-- View character
displayCharacterSheet()
displayEquipment()

-- Combat
enterCombat()
castAbility("Fireball")  -- Costs mana

-- Level up
addExperience(1500)
increaseStat("INT", 3)

leaveCombat()  -- Faster regen
```

### Fighter Dual-Wield

```lua
setPlayerClass("FIGHTER")
equipWeapon(sword1, EquipmentSlot.MAIN_HAND, PlayerCharacter.class)
equipWeapon(sword2, EquipmentSlot.OFF_HAND, PlayerCharacter.class)
castAbility("Whirlwind")
```

### Priest Healer

```lua
setPlayerClass("PRIEST")
equipWeapon(mace, EquipmentSlot.MAIN_HAND, PlayerCharacter.class)
equipWeapon(shield, EquipmentSlot.OFF_HAND, PlayerCharacter.class)
castAbility("Heal")
castAbility("Mass Heal")
```

## D&D Stats Explained

- **STR** - Melee damage (Fighter)
- **DEX** - Ranged, AC (Ranger)
- **CON** - Health (All)
- **INT** - Wizard spells
- **WIS** - Priest/Druid spells
- **CHA** - Sorcerer/Warlock spells

## Additional Plugins

### 3. custom_blocks.lua
8 new block types: Obsidian, Ores (Gold/Diamond/Emerald), Glass, Brick, Ice, Lava

### 4. items.lua  
Materials, tools, consumables (legacy - use equipment_system for RPG items)

### 5-7. armor.lua, weapons.lua, player_character.lua
Legacy plugins - superseded by character_classes.lua and equipment_system.lua

### 8. example.lua
Basic plugin template

## Plugin API

```lua
-- Logging
log(message)

-- Blocks
getBlockType(x, y, z)
setBlockType(x, y, z, type)

-- Hooks
function onLoad() end
function onUpdate(deltaTime) end
function onInput() end
function onRender() end
```

## Security

✅ Allowed: math, string, table, log(), block API
❌ Blocked: file I/O, system commands, code loading, debug, network

## Class Balance

| Class | Primary | Health (Base+/lvl) | Resource | Special |
|-------|---------|-----|----------|---------|
| Wizard | INT | 40+6 | High Mana | Pure DPS |
| Sorcerer | CHA | 45+6 | Highest Mana | Versatile |
| Priest | WIS | 50+8 | Medium Mana | Healer |
| Fighter | STR | 70+10 | Medium Energy | Tank/DPS |
| Ranger | DEX | 60+10 | Medium Focus | DPS+Pet |
| Warlock | CHA | 50+8 | Medium Soul | DPS+Pet |
| Druid | WIS | 55+8 | Medium Nature | Hybrid |

## License

Part of CloneMine - free to use and modify.
