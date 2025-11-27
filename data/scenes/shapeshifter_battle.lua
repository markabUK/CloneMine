-- Example scripted scene: Shapeshifter Boss Battle
-- Demonstrates shape-changing, chained actions, and various death types
-- Triggered when player enters the shapeshifter's lair

local scene = SceneManager:createScene("shapeshifter_battle")

-- Trigger on location enter
SceneManager:addTrigger(scene, "LOCATION_ENTER", "shapeshifter_lair")

-- Fade to dramatic lighting
SceneManager:addAction(scene, {
    type = "FADE_OUT",
    duration = 1.0
})

-- Move camera for cinematic intro
SceneManager:addAction(scene, {
    type = "MOVE_CAMERA",
    vec3Param = {x = 0, y = 15, z = 25},
    duration = 1.0
})

SceneManager:addAction(scene, {
    type = "FADE_IN",
    duration = 1.5
})

-- Spawn the shapeshifter (starts in human form)
SceneManager:addAction(scene, {
    type = "SPAWN_ENTITY",
    stringParam = "shapeshifter_boss",
    vec3Param = {x = 20, y = 0, z = 20},
    duration = 1.0
})

-- Boss introduction
SceneManager:addAction(scene, {
    type = "DIALOG",
    duration = 4.0,
    dialogLines = {{
        speakerName = "Shapeshifter",
        text = "Foolish mortal! You cannot defeat my many forms!",
        audioFile = "dialog/shapeshifter_intro.ogg"
    }}
})

-- Wait
SceneManager:addAction(scene, {
    type = "WAIT",
    duration = 1.0
})

-- Phase 1: Human form attack
SceneManager:addAction(scene, {
    type = "ATTACK",
    stringParam = "shapeshifter_boss",
    stringParam2 = "player",
    duration = 1.0
})

-- Player dodges and counter-attacks
SceneManager:addAction(scene, {
    type = "WAIT",
    duration = 0.5
})

SceneManager:addAction(scene, {
    type = "ATTACK",
    stringParam = "player",
    stringParam2 = "shapeshifter_boss",
    duration = 1.0
})

SceneManager:addAction(scene, {
    type = "TAKE_DAMAGE",
    stringParam = "shapeshifter_boss",
    intParam = 100,
    duration = 0.5
})

-- Boss transforms into bear form
SceneManager:addAction(scene, {
    type = "DIALOG",
    duration = 2.5,
    dialogLines = {{
        speakerName = "Shapeshifter",
        text = "Witness my true power!",
        audioFile = "dialog/shapeshifter_transform.ogg"
    }}
})

SceneManager:addAction(scene, {
    type = "SHAPE_CHANGE",
    stringParam = "shapeshifter_boss",
    stringParam2 = "bear",
    duration = 2.0
})

SceneManager:addAction(scene, {
    type = "PLAY_AUDIO",
    stringParam = "transformation_effect",
    duration = 0.1
})

-- Phase 2: Bear form - more aggressive
SceneManager:addAction(scene, {
    type = "SHOW_TEXT",
    stringParam = "The Shapeshifter transforms into a massive bear!",
    duration = 3.0
})

-- Powerful bear attack
SceneManager:addAction(scene, {
    type = "ATTACK",
    stringParam = "shapeshifter_boss",
    stringParam2 = "player",
    duration = 1.0
})

-- Player takes heavy damage and gets knocked back
SceneManager:addAction(scene, {
    type = "TAKE_DAMAGE",
    stringParam = "player",
    intParam = 75,
    duration = 0.5
})

SceneManager:addAction(scene, {
    type = "KNOCKBACK",
    stringParam = "player",
    vec3Param = {x = -1, y = 0.5, z = -1},  -- knockback direction
    floatParam = 15.0,  -- force
    duration = 1.0
})

SceneManager:addAction(scene, {
    type = "PLAY_AUDIO",
    stringParam = "bear_roar",
    duration = 0.1
})

-- Player recovers and casts healing spell
SceneManager:addAction(scene, {
    type = "WAIT",
    duration = 1.0
})

SceneManager:addAction(scene, {
    type = "CAST_SPELL",
    stringParam = "player",
    stringParam2 = "heal",
    duration = 1.5
})

SceneManager:addAction(scene, {
    type = "HEAL",
    stringParam = "player",
    intParam = 50,
    duration = 1.0
})

-- Player attacks bear form
SceneManager:addAction(scene, {
    type = "ATTACK",
    stringParam = "player",
    stringParam2 = "shapeshifter_boss",
    duration = 1.0
})

SceneManager:addAction(scene, {
    type = "ATTACK",
    stringParam = "player",
    stringParam2 = "shapeshifter_boss",
    duration = 1.0
})

SceneManager:addAction(scene, {
    type = "TAKE_DAMAGE",
    stringParam = "shapeshifter_boss",
    intParam = 150,
    duration = 0.5
})

-- Boss transforms again into dragon form
SceneManager:addAction(scene, {
    type = "SHAPE_CHANGE",
    stringParam = "shapeshifter_boss",
    stringParam2 = "dragon",
    duration = 3.0
})

SceneManager:addAction(scene, {
    type = "PLAY_AUDIO",
    stringParam = "dragon_roar",
    duration = 0.1
})

-- Phase 3: Dragon form - final phase
SceneManager:addAction(scene, {
    type = "SHOW_TEXT",
    stringParam = "The Shapeshifter becomes a fearsome dragon!",
    duration = 3.0
})

-- Dragon breathes fire - area effect
SceneManager:addAction(scene, {
    type = "CAST_SPELL",
    stringParam = "shapeshifter_boss",
    stringParam2 = "fire_breath",
    duration = 2.0
})

SceneManager:addAction(scene, {
    type = "AREA_EFFECT",
    stringParam = "fire",
    vec3Param = {x = 5, y = 0, z = 5},  -- area in front of dragon
    floatParam = 8.0,  -- radius
    intParam = 80,     -- power
    duration = 2.0
})

SceneManager:addAction(scene, {
    type = "PLAY_AUDIO",
    stringParam = "dragon_breath",
    duration = 0.1
})

-- Player barely dodges
SceneManager:addAction(scene, {
    type = "WAIT",
    duration = 1.0
})

-- Player casts powerful lightning spell
SceneManager:addAction(scene, {
    type = "CAST_SPELL",
    stringParam = "player",
    stringParam2 = "lightning",
    vec3Param = {x = 20, y = 0, z = 20},  -- dragon position
    duration = 1.5
})

SceneManager:addAction(scene, {
    type = "PLAY_AUDIO",
    stringParam = "lightning_cast",
    duration = 0.1
})

-- Lightning strikes dragon
SceneManager:addAction(scene, {
    type = "AREA_EFFECT",
    stringParam = "lightning",
    vec3Param = {x = 20, y = 0, z = 20},
    floatParam = 3.0,
    intParam = 250,
    duration = 1.0
})

SceneManager:addAction(scene, {
    type = "TAKE_DAMAGE",
    stringParam = "shapeshifter_boss",
    stringParam2 = "lightning",
    intParam = 250,
    duration = 0.5
})

-- Boss is defeated
SceneManager:addAction(scene, {
    type = "DIALOG",
    duration = 3.0,
    dialogLines = {{
        speakerName = "Shapeshifter",
        text = "Impossible... defeated by a mere mortal...",
        audioFile = "dialog/shapeshifter_defeat.ogg"
    }}
})

-- Dragon dies and crumbles
SceneManager:addAction(scene, {
    type = "DIE",
    stringParam = "shapeshifter_boss",
    stringParam2 = "crumble",  -- body turns to stone and crumbles
    duration = 3.0
})

SceneManager:addAction(scene, {
    type = "PLAY_AUDIO",
    stringParam = "creature_death",
    duration = 0.1
})

-- Show victory
SceneManager:addAction(scene, {
    type = "SHOW_TEXT",
    stringParam = "The Shapeshifter has been defeated!",
    duration = 4.0
})

-- Return camera to player
SceneManager:addAction(scene, {
    type = "SET_CAMERA_TARGET",
    stringParam = "player",
    duration = 1.0
})

-- Fade to normal
SceneManager:addAction(scene, {
    type = "FADE_OUT",
    duration = 0.5
})

SceneManager:addAction(scene, {
    type = "FADE_IN",
    duration = 1.0
})

-- Rewards
SceneManager:addAction(scene, {
    type = "GIVE_ITEM",
    stringParam = "shapeshifter_amulet",
    intParam = 1,
    duration = 0.1
})

SceneManager:addAction(scene, {
    type = "GIVE_ITEM",
    stringParam = "gold",
    intParam = 500,
    duration = 0.1
})

SceneManager:addAction(scene, {
    type = "COMPLETE_QUEST",
    stringParam = "defeat_shapeshifter",
    duration = 0.1
})

-- Final message
SceneManager:addAction(scene, {
    type = "SHOW_TEXT",
    stringParam = "Quest Complete: The Shapeshifter's reign of terror has ended!",
    duration = 4.0
})

print("Loaded scene: shapeshifter_battle")
