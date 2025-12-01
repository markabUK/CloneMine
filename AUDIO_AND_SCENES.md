# Audio and Scripted Scenes System

## Overview

CloneMine now features a comprehensive audio system and scripted scene framework for creating immersive storytelling experiences.

## Audio System

### Features

- **Cross-platform audio** using OpenAL Soft
- **3D positional audio** for spatial sound effects
- **Multiple audio categories** with independent volume control
- **Event-based audio** system for easy triggering
- **Support for OGG/WAV** audio formats

### Audio Categories

1. **WEAPON** - Sword swings, bow shots, impacts
2. **SPELL** - Spell casting and impact sounds
3. **CREATURE** - Monster idle sounds, attacks, death cries
4. **PLAYER** - Player hurt/death, footsteps, jumps
5. **DIALOG** - NPC dialog audio
6. **AMBIENT** - Environmental sounds
7. **MUSIC** - Background music
8. **UI** - Menu and interface sounds

### Audio Events

The system includes pre-defined audio events that can be triggered anywhere in the game:

**Weapon Events:**
- `SWORD_SWING` - Play when swinging sword
- `SWORD_HIT` - Play on successful hit
- `BOW_SHOOT` - Play when firing bow
- `ARROW_HIT` - Play when arrow impacts

**Spell Events:**
- `FIREBALL_CAST` - Fireball casting sound
- `FIREBALL_IMPACT` - Fireball explosion
- `HEAL_CAST` - Healing spell cast
- `LIGHTNING_CAST` - Lightning spell cast

**Creature Events:**
- `ZOMBIE_IDLE` - Zombie ambient sounds
- `ZOMBIE_ATTACK` - Zombie attacking
- `ZOMBIE_DEATH` - Zombie death cry
- `SKELETON_IDLE` - Skeleton rattling
- `SKELETON_DEATH` - Skeleton destruction

**Player Events:**
- `PLAYER_HURT` - Player taking damage
- `PLAYER_DEATH` - Player death sound
- `PLAYER_LEVEL_UP` - Level up fanfare
- `FOOTSTEP` - Footstep sound
- `JUMP` - Jump sound
- `LAND` - Landing sound

**Dialog/UI Events:**
- `DIALOG_OPEN` - Dialog window opens
- `DIALOG_CLOSE` - Dialog window closes
- `QUEST_ACCEPT` - Quest accepted sound
- `QUEST_COMPLETE` - Quest completion fanfare

### Using the Audio System

#### Initialize

```cpp
#include "audio/AudioManager.h"

auto audioManager = std::make_unique<clonemine::audio::AudioManager>();
audioManager->initialize();
```

#### Load Audio

```cpp
// Load individual sounds
audioManager->loadSound("sword_swing", "audio/weapons/sword_swing.ogg", 
                        clonemine::audio::AudioCategory::WEAPON);

// Load music
audioManager->loadMusic("boss_music", "audio/music/boss_battle.ogg");
```

#### Play Sounds

```cpp
// Play 2D sound (UI, music)
audioManager->playSound("quest_accept");

// Play 3D positional sound
glm::vec3 enemyPosition(10.0f, 0.0f, 5.0f);
audioManager->playSound3D("zombie_death", enemyPosition);

// Play audio event
audioManager->playEvent(clonemine::audio::AudioEvent::SWORD_HIT, hitPosition);
```

#### Control Music

```cpp
// Play looping background music
audioManager->playMusic("boss_music", true);

// Control playback
audioManager->pauseMusic();
audioManager->resumeMusic();
audioManager->stopMusic();
```

#### Volume Control

```cpp
// Set master volume (0.0 to 1.0)
audioManager->setMasterVolume(0.8f);

// Set category-specific volume
audioManager->setCategoryVolume(clonemine::audio::AudioCategory::MUSIC, 0.5f);
```

#### Update Listener (Player Position)

```cpp
// Update every frame for 3D audio
glm::vec3 playerPos = player->getPosition();
glm::vec3 playerVel = player->getVelocity();
glm::vec3 forward = player->getForward();
glm::vec3 up(0.0f, 1.0f, 0.0f);

audioManager->setListenerPosition(playerPos);
audioManager->setListenerVelocity(playerVel);
audioManager->setListenerOrientation(forward, up);
```

## Scripted Scenes System

### Features

- **Lua-based scene scripting** for easy content creation
- **Multiple trigger types** (quests, NPCs, locations, etc.)
- **Rich action types** (dialog, camera, audio, etc.)
- **Dialog with audio support** for voice acting
- **Sequential action execution** with timing control

### Trigger Types

- `QUEST_START` - Triggered when quest begins
- `QUEST_COMPLETE` - Triggered when quest completes
- `QUEST_PROGRESS` - Triggered on quest progress
- `NPC_INTERACTION` - Triggered when talking to NPC
- `LOCATION_ENTER` - Triggered when entering location
- `ITEM_PICKUP` - Triggered when picking up item
- `ENEMY_DEFEAT` - Triggered when defeating enemy
- `LEVEL_UP` - Triggered on level up
- `MANUAL` - Manually triggered

### Scene Actions

1. **DIALOG** - Show dialog with optional audio
2. **MOVE_PLAYER** - Move player to position
3. **MOVE_CAMERA** - Move camera for cutscene
4. **PLAY_ANIMATION** - Play character animation
5. **PLAY_AUDIO** - Play sound effect or music
6. **SPAWN_ENTITY** - Spawn NPC or object
7. **WAIT** - Wait for duration
8. **FADE_IN/FADE_OUT** - Screen fades
9. **SET_CAMERA_TARGET** - Point camera at target
10. **SHOW_TEXT** - Display on-screen text
11. **GIVE_ITEM** - Give item to player
12. **COMPLETE_QUEST** - Mark quest complete

### Using Scripted Scenes

#### Initialize Scene Manager

```cpp
#include "scripting/ScriptedScene.h"

auto sceneManager = std::make_unique<clonemine::scripting::SceneManager>();
sceneManager->initialize(audioManager.get());
```

#### Load Scene Scripts

```cpp
// Load scene from Lua file
sceneManager->loadScene("data/scenes/tutorial_quest_start.lua");
sceneManager->loadScene("data/scenes/zombie_quest_complete.lua");
sceneManager->loadScene("data/scenes/boss_battle_intro.lua");
```

#### Trigger Scenes

```cpp
// Trigger scenes based on game events
sceneManager->triggerScenes(
    clonemine::scripting::SceneTriggerType::QUEST_START, 
    "tutorial_quest"
);

sceneManager->triggerScenes(
    clonemine::scripting::SceneTriggerType::QUEST_COMPLETE, 
    "zombie_slayer_1"
);

sceneManager->triggerScenes(
    clonemine::scripting::SceneTriggerType::LOCATION_ENTER, 
    "dungeon_boss_room"
);
```

#### Update Every Frame

```cpp
// In game loop
float deltaTime = calculateDeltaTime();
audioManager->update(deltaTime);
sceneManager->update(deltaTime);
```

### Creating Scene Scripts

Scene scripts are written in Lua. See `data/scenes/` for examples.

#### Basic Scene Structure

```lua
-- Create scene
local scene = SceneManager:createScene("my_scene_name")

-- Add trigger
SceneManager:addTrigger(scene, "QUEST_START", "my_quest_id")

-- Add actions
SceneManager:addAction(scene, {
    type = "DIALOG",
    duration = 5.0,
    dialogLines = {
        {
            speakerName = "NPC Name",
            text = "Hello, adventurer!",
            audioFile = "dialog/npc_hello.ogg",
            displayTime = 5.0
        }
    }
})

SceneManager:addAction(scene, {
    type = "PLAY_AUDIO",
    stringParam = "quest_accept",
    duration = 0.1
})

SceneManager:addAction(scene, {
    type = "SHOW_TEXT",
    stringParam = "New Quest Accepted!",
    duration = 3.0
})
```

### Example: Quest Start Scene

```lua
local scene = SceneManager:createScene("tutorial_quest_start")
SceneManager:addTrigger(scene, "QUEST_START", "tutorial_quest")

-- Fade in
SceneManager:addAction(scene, {type = "FADE_IN", duration = 2.0})

-- Play music
SceneManager:addAction(scene, {
    type = "PLAY_AUDIO",
    stringParam = "tutorial_music",
    duration = 0.1
})

-- NPC dialog with voice
SceneManager:addAction(scene, {
    type = "DIALOG",
    duration = 5.0,
    dialogLines = {{
        speakerName = "Village Elder",
        text = "Welcome, young adventurer! I have a quest for you.",
        audioFile = "tutorial/elder_welcome.ogg",
        displayTime = 5.0
    }}
})

-- Accept sound
SceneManager:addAction(scene, {
    type = "PLAY_AUDIO",
    stringParam = "quest_accept",
    duration = 0.1
})

-- Show quest name
SceneManager:addAction(scene, {
    type = "SHOW_TEXT",
    stringParam = "New Quest: Zombie Slayer I",
    duration = 3.0
})
```

## Audio File Organization

Place audio files in the following structure:

```
data/audio/
├── weapons/
│   ├── sword_swing.ogg
│   ├── sword_hit.ogg
│   ├── bow_shoot.ogg
│   └── arrow_hit.ogg
├── spells/
│   ├── fireball_cast.ogg
│   ├── fireball_impact.ogg
│   ├── heal_cast.ogg
│   └── lightning_cast.ogg
├── creatures/
│   ├── zombie_idle.ogg
│   ├── zombie_attack.ogg
│   ├── zombie_death.ogg
│   ├── skeleton_idle.ogg
│   ├── skeleton_death.ogg
│   └── boss_roar.ogg
├── player/
│   ├── hurt.ogg
│   ├── death.ogg
│   ├── level_up.ogg
│   ├── footstep.ogg
│   ├── jump.ogg
│   └── land.ogg
├── ui/
│   ├── dialog_open.ogg
│   ├── dialog_close.ogg
│   ├── quest_accept.ogg
│   └── quest_complete.ogg
├── music/
│   ├── tutorial.ogg
│   ├── boss_battle.ogg
│   └── victory.ogg
└── dialog/
    └── tutorial/
        ├── elder_welcome.ogg
        ├── player_ready.ogg
        ├── elder_quest.ogg
        └── elder_congratulate.ogg
```

## Audio Format Recommendations

- **Format**: OGG Vorbis (best compression, open source)
- **Fallback**: WAV (uncompressed, larger files)
- **Sample Rate**: 44.1 kHz
- **Bit Depth**: 16-bit
- **Channels**: Mono for 3D positional sounds, Stereo for music/ambient

### Sound Effect Guidelines

- **Weapon sounds**: 0.5-2 seconds, mono
- **Spell sounds**: 1-3 seconds, mono for casts, stereo for impacts
- **Creature sounds**: 1-4 seconds, mono
- **Player sounds**: 0.5-2 seconds, mono
- **Dialog**: Variable length, mono or stereo
- **Music**: 2-5 minutes looping, stereo

## Integration with Game Systems

### Combat System

```cpp
// In combat code
void DamageCalculation::applyDamage(...) {
    // Play weapon hit sound
    audioManager->playEvent(
        clonemine::audio::AudioEvent::SWORD_HIT,
        target->getPosition()
    );
    
    if (target->isDead()) {
        // Play death sound
        audioManager->playEvent(
            clonemine::audio::AudioEvent::ZOMBIE_DEATH,
            target->getPosition()
        );
    }
}
```

### Quest System

```cpp
// When quest starts
void QuestManager::startQuest(const std::string& questId) {
    // Trigger scene
    sceneManager->triggerScenes(
        clonemine::scripting::SceneTriggerType::QUEST_START,
        questId
    );
}

// When quest completes
void QuestManager::completeQuest(const std::string& questId) {
    // Play completion sound
    audioManager->playEvent(clonemine::audio::AudioEvent::QUEST_COMPLETE);
    
    // Trigger completion scene
    sceneManager->triggerScenes(
        clonemine::scripting::SceneTriggerType::QUEST_COMPLETE,
        questId
    );
}
```

### Player Actions

```cpp
// Jump
void Player::jump() {
    audioManager->playEvent(
        clonemine::audio::AudioEvent::JUMP,
        getPosition()
    );
}

// Footsteps (call every N steps)
void Player::updateMovement(float deltaTime) {
    if (isMoving && shouldPlayFootstep()) {
        audioManager->playEvent(
            clonemine::audio::AudioEvent::FOOTSTEP,
            getPosition()
        );
    }
}
```

### NPC Interaction

```cpp
void NPC::interact() {
    // Trigger interaction scene
    sceneManager->triggerScenes(
        clonemine::scripting::SceneTriggerType::NPC_INTERACTION,
        getNpcId()
    );
}
```

## Performance Considerations

- Audio system uses OpenAL for hardware acceleration
- 3D audio calculations are efficient (handled by OpenAL)
- Scene scripts are preloaded (minimal runtime overhead)
- Audio streaming for large music files (not yet implemented)
- Maximum ~64 simultaneous 3D sources (OpenAL limit)

## Future Enhancements

- [ ] Audio streaming for long music tracks
- [ ] Dynamic music system (intensity-based layers)
- [ ] Reverb and environmental effects
- [ ] Audio occlusion (walls blocking sound)
- [ ] Lip sync for dialog
- [ ] Subtitle system
- [ ] Voice chat integration (multiplayer)
- [ ] Audio compression options
- [ ] In-game audio mixer UI

## Troubleshooting

**No audio playing:**
- Check OpenAL is installed
- Verify audio files exist at specified paths
- Check master/category volumes not set to 0
- Ensure AudioManager::initialize() succeeded

**3D audio not working:**
- Verify listener position is being updated
- Check listener orientation is set correctly
- Ensure sounds are marked as 3D (not UI sounds)

**Scenes not triggering:**
- Verify scene files are loaded successfully
- Check trigger type and ID match exactly
- Ensure SceneManager::update() is being called
- Check Lua script syntax

## See Also

- Example scenes in `data/scenes/`
- Audio manifest in `data/audio/audio_manifest.txt`
- Lua API documentation
- OpenAL Soft documentation
