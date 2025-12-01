# Scripted Combat and Actions System

## Overview

The scripted scenes system now supports comprehensive combat actions, allowing you to create detailed scripted fights, boss battles, and dramatic story moments with full control over attacks, spells, abilities, deaths, and special effects.

## New Combat Action Types

### Basic Combat Actions

#### ATTACK
Entity performs an attack action against a target.

**Parameters:**
- `stringParam`: Attacker entity ID
- `stringParam2`: Target entity ID  
- `duration`: Attack animation time (default: 1.0s)

**Example:**
```lua
SceneManager:addAction(scene, {
    type = "ATTACK",
    stringParam = "player",
    stringParam2 = "evil_wizard",
    duration = 1.0
})
```

#### CAST_SPELL
Entity casts a spell at a target or position.

**Parameters:**
- `stringParam`: Caster entity ID
- `stringParam2`: Spell name (e.g., "fireball", "heal", "lightning")
- `vec3Param`: Target position
- `duration`: Cast time (default: 2.0s)

**Example:**
```lua
SceneManager:addAction(scene, {
    type = "CAST_SPELL",
    stringParam = "evil_wizard",
    stringParam2 = "disintegration",
    vec3Param = {x = -5, y = 0, z = -5},
    duration = 3.0
})
```

#### USE_ABILITY
Entity uses a special ability.

**Parameters:**
- `stringParam`: Entity ID
- `stringParam2`: Ability name (e.g., "berserk_rage", "shield_bash")
- `duration`: Ability activation time (default: 1.0s)

**Example:**
```lua
SceneManager:addAction(scene, {
    type = "USE_ABILITY",
    stringParam = "player",
    stringParam2 = "berserk_rage",
    duration = 1.5
})
```

### Damage and Healing

#### TAKE_DAMAGE
Entity takes damage.

**Parameters:**
- `stringParam`: Entity ID
- `stringParam2`: Damage type ("physical", "fire", "lightning", etc.)
- `intParam`: Damage amount
- `duration`: Hurt animation time (default: 0.5s)

**Example:**
```lua
SceneManager:addAction(scene, {
    type = "TAKE_DAMAGE",
    stringParam = "player",
    stringParam2 = "fire",
    intParam = 50,
    duration = 0.5
})
```

#### HEAL
Entity is healed.

**Parameters:**
- `stringParam`: Entity ID
- `intParam`: Heal amount
- `duration`: Healing effect time (default: 1.0s)

**Example:**
```lua
SceneManager:addAction(scene, {
    type = "HEAL",
    stringParam = "player",
    intParam = 50,
    duration = 1.0
})
```

### Death and Destruction

#### DIE
Entity dies with a specific death type.

**Parameters:**
- `stringParam`: Entity ID
- `stringParam2`: Death type:
  - `"normal"` - Standard death animation
  - `"explode"` - Entity explodes
  - `"vaporize"` - Entity is vaporized
  - `"disintegrate"` - Entity crumbles to dust
  - `"crumble"` - Entity turns to stone and crumbles
- `duration`: Death animation time (default: 2.0s)

**Example:**
```lua
SceneManager:addAction(scene, {
    type = "DIE",
    stringParam = "evil_wizard",
    stringParam2 = "explode",
    duration = 2.0
})
```

#### DESTROY_ENTITY
Instantly destroy/remove entity with special effect.

**Parameters:**
- `stringParam`: Entity ID
- `stringParam2`: Destruction type ("vaporize", "disintegrate", "crumble", "explode")
- `duration`: Effect duration (default: 1.0s)

**Example:**
```lua
-- Weak enemy hit by disintegration spell
SceneManager:addAction(scene, {
    type = "DESTROY_ENTITY",
    stringParam = "weak_goblin",
    stringParam2 = "vaporize",
    duration = 1.0
})
```

### Special Effects

#### AREA_EFFECT
Create an area of effect at a location.

**Parameters:**
- `stringParam`: Effect type ("explosion", "disintegration", "fire", "lightning", "freeze", "poison")
- `vec3Param`: Center position
- `floatParam`: Radius
- `intParam`: Power/damage
- `duration`: Effect duration (default: 3.0s)

**Example:**
```lua
-- Disintegration area effect
SceneManager:addAction(scene, {
    type = "AREA_EFFECT",
    stringParam = "disintegration",
    vec3Param = {x = -5, y = 0, z = -5},
    floatParam = 3.0,  -- 3 meter radius
    intParam = 200,    -- 200 damage
    duration = 2.0
})
```

#### SHAPE_CHANGE
Entity transforms into another form.

**Parameters:**
- `stringParam`: Entity ID
- `stringParam2`: New form ("bear", "wolf", "dragon", etc.)
- `duration`: Transformation time (default: 2.0s)

**Example:**
```lua
SceneManager:addAction(scene, {
    type = "SHAPE_CHANGE",
    stringParam = "shapeshifter_boss",
    stringParam2 = "dragon",
    duration = 3.0
})
```

#### KNOCKBACK
Apply knockback force to entity.

**Parameters:**
- `stringParam`: Entity ID
- `vec3Param`: Knockback direction (normalized vector)
- `floatParam`: Force strength
- `duration`: Knockback animation time (default: 1.0s)

**Example:**
```lua
SceneManager:addAction(scene, {
    type = "KNOCKBACK",
    stringParam = "player",
    vec3Param = {x = -1, y = 0.5, z = -1},
    floatParam = 15.0,
    duration = 1.0
})
```

#### TELEPORT
Instantly teleport entity to new position.

**Parameters:**
- `stringParam`: Entity ID
- `vec3Param`: Target position
- `duration`: Teleport effect time (default: 0.5s)

**Example:**
```lua
SceneManager:addAction(scene, {
    type = "TELEPORT",
    stringParam = "boss",
    vec3Param = {x = 10, y = 0, z = 10},
    duration = 0.5
})
```

### Status Effects

#### APPLY_STATUS
Apply a status effect (buff/debuff) to entity.

**Parameters:**
- `stringParam`: Entity ID
- `stringParam2`: Status name ("poison", "stun", "slow", "haste", "berserk", "shield", etc.)
- `floatParam`: Status effect duration
- `duration`: Application time (default: 0.5s)

**Example:**
```lua
SceneManager:addAction(scene, {
    type = "APPLY_STATUS",
    stringParam = "player",
    stringParam2 = "berserk",
    floatParam = 10.0,  -- lasts 10 seconds
    duration = 0.5
})
```

#### REMOVE_STATUS
Remove a status effect from entity.

**Parameters:**
- `stringParam`: Entity ID
- `stringParam2`: Status name
- `duration`: Removal time (default: 0.5s)

**Example:**
```lua
SceneManager:addAction(scene, {
    type = "REMOVE_STATUS",
    stringParam = "player",
    stringParam2 = "berserk",
    duration = 0.5
})
```

### Action Chaining

#### CHAIN_ACTIONS
Execute multiple actions in sequence on the same entity.

**Parameters:**
- `chainedActions`: Vector of SceneAction objects
- `duration`: Automatically calculated from chained actions

**Example (C++):**
```cpp
std::vector<SceneAction> combo;
combo.push_back(SceneBuilder::createAttackAction("player", "enemy", "slash"));
combo.push_back(SceneBuilder::createAttackAction("player", "enemy", "slash"));
combo.push_back(SceneBuilder::createAttackAction("player", "enemy", "slash"));

SceneAction chainedCombo = SceneBuilder::createChainedAction(combo);
scene->addAction(chainedCombo);
```

## Complete Examples

### Example 1: Wizard Duel

See `data/scenes/wizard_duel.lua` for a complete scripted wizard battle featuring:
- Spell casting (fireball, disintegration)
- Attack sequences
- Area effect disintegration that vaporizes weak enemies
- Player using abilities (berserk rage)
- Boss death with explosion effect
- Quest completion and rewards

Key highlights:
```lua
-- Wizard casts disintegration, weak enemy vaporizes
SceneManager:addAction(scene, {
    type = "CAST_SPELL",
    stringParam = "evil_wizard",
    stringParam2 = "disintegration",
    vec3Param = {x = -5, y = 0, z = -5},
    duration = 3.0
})

SceneManager:addAction(scene, {
    type = "AREA_EFFECT",
    stringParam = "disintegration",
    vec3Param = {x = -5, y = 0, z = -5},
    floatParam = 3.0,
    intParam = 200,
    duration = 2.0
})

SceneManager:addAction(scene, {
    type = "DESTROY_ENTITY",
    stringParam = "weak_goblin",
    stringParam2 = "vaporize",
    duration = 1.0
})
```

### Example 2: Shapeshifter Boss Battle

See `data/scenes/shapeshifter_battle.lua` for a multi-phase boss fight featuring:
- Shape-changing boss (human → bear → dragon)
- Different attack patterns per form
- Area effect fire breath
- Knockback effects
- Healing spells
- Death with crumbling effect
- Epic cinematic camera work

Key highlights:
```lua
-- Boss transforms into dragon
SceneManager:addAction(scene, {
    type = "SHAPE_CHANGE",
    stringParam = "shapeshifter_boss",
    stringParam2 = "dragon",
    duration = 3.0
})

-- Dragon breathes fire (area effect)
SceneManager:addAction(scene, {
    type = "AREA_EFFECT",
    stringParam = "fire",
    vec3Param = {x = 5, y = 0, z = 5},
    floatParam = 8.0,
    intParam = 80,
    duration = 2.0
})

-- Dragon dies and crumbles
SceneManager:addAction(scene, {
    type = "DIE",
    stringParam = "shapeshifter_boss",
    stringParam2 = "crumble",
    duration = 3.0
})
```

## Scene Triggers for Combat

Combat scenes can be triggered by:

- **QUEST_START/QUEST_COMPLETE** - Begin/end quest battles
- **NPC_INTERACTION** - Talk to NPC initiates fight
- **LOCATION_ENTER** - Enter boss room triggers battle
- **ENEMY_DEFEAT** - Defeating one enemy triggers another
- **LEVEL_UP** - Reaching milestone triggers challenge

## Best Practices

### 1. Timing
- Allow appropriate time for animations
- Add WAIT actions between rapid sequences
- Use PLAY_AUDIO to enhance impact

### 2. Death Types
Choose death type based on the damage source:
- **Physical damage** → "normal"
- **Fire/Explosion** → "explode"
- **Disintegration spell** → "vaporize"
- **Petrification** → "crumble"
- **Entropy/decay** → "disintegrate"

### 3. Area Effects
- Match area effect type to spell/ability
- Use appropriate radius (3-10m typical)
- Combine with audio for immersion
- Follow with TAKE_DAMAGE or DESTROY_ENTITY for affected entities

### 4. Status Effects
Common status combos:
- **Berserk** → increased damage, attack speed
- **Stun** → can't move or act
- **Slow** → reduced movement/attack speed
- **Haste** → increased speed
- **Poison** → damage over time
- **Shield** → damage reduction

### 5. Chaining Actions
Use action chains for:
- Combo attacks
- Multi-step spells
- Complex transformations
- Synchronized team attacks

## Integration with Combat System

These scripted actions integrate with the game's combat system:

1. **Damage Calculation**: TAKE_DAMAGE respects entity's armor/resistances
2. **Status Effects**: APPLY_STATUS uses game's status system
3. **Death Handling**: DIE triggers proper loot drops and XP
4. **Audio**: All actions automatically trigger appropriate AudioEvents
5. **Animation**: Actions trigger corresponding entity animations

## Performance Considerations

- **Scene Complexity**: Keep scenes under 100 actions for smooth playback
- **Area Effects**: Limit simultaneous area effects to 3-4
- **Chained Actions**: Chains of 5-10 actions work best
- **Update Frequency**: Scenes update at game tick rate (60 Hz)

## Future Enhancements

Planned features:
- Conditional branching in scenes
- Player choice dialogs affecting combat
- Dynamic difficulty scaling
- AI-controlled scene participants
- Multiplayer synchronized scenes
- Procedurally generated combat encounters

## Troubleshooting

**Problem**: Entity doesn't respond to action
- **Solution**: Verify entity ID is correct and entity is spawned

**Problem**: Death effect doesn't play
- **Solution**: Ensure PLAY_AUDIO action follows DIE action

**Problem**: Area effect hits wrong targets
- **Solution**: Check vec3Param position and floatParam radius

**Problem**: Status effect doesn't apply
- **Solution**: Verify status name matches game's status system

## Reference

See these files for complete examples:
- `data/scenes/wizard_duel.lua` - Spell combat with disintegration
- `data/scenes/shapeshifter_battle.lua` - Shape-changing boss
- `data/scenes/boss_battle_intro.lua` - Basic boss introduction
- `src/scripting/ScriptedScene.h` - Action type definitions
- `src/scripting/ScriptedScene.cpp` - Action implementations

