-- Example scripted scene: Boss Battle Introduction
-- Triggered when player enters the dungeon boss room

local scene = SceneManager:createScene("boss_battle_intro")

-- Trigger on location enter
SceneManager:addTrigger(scene, "LOCATION_ENTER", "dungeon_boss_room")

-- Fade to black
SceneManager:addAction(scene, {
    type = "FADE_OUT",
    duration = 1.0
})

-- Move camera for dramatic angle
SceneManager:addAction(scene, {
    type = "MOVE_CAMERA",
    vec3Param = {x = 0, y = 10, z = 20},
    duration = 0.5
})

-- Fade in
SceneManager:addAction(scene, {
    type = "FADE_IN",
    duration = 1.5
})

-- Play ominous music
SceneManager:addAction(scene, {
    type = "PLAY_AUDIO",
    stringParam = "boss_music",
    duration = 0.1
})

-- Wait for dramatic effect
SceneManager:addAction(scene, {
    type = "WAIT",
    duration = 2.0
})

-- Boss roar sound
SceneManager:addAction(scene, {
    type = "PLAY_AUDIO",
    stringParam = "boss_roar",
    duration = 0.1
})

-- Show boss name
SceneManager:addAction(scene, {
    type = "SHOW_TEXT",
    stringParam = "Ancient Guardian Awakens...",
    duration = 3.0
})

-- Wait
SceneManager:addAction(scene, {
    type = "WAIT",
    duration = 1.0
})

-- Return camera to player
SceneManager:addAction(scene, {
    type = "SET_CAMERA_TARGET",
    stringParam = "player",
    duration = 1.0
})

print("Loaded scene: boss_battle_intro")
