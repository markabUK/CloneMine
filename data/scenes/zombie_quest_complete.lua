-- Example scripted scene: Quest Complete Celebration
-- This scene is triggered when the player completes the zombie quest

local scene = SceneManager:createScene("zombie_quest_complete")

-- Set up trigger
SceneManager:addTrigger(scene, "QUEST_COMPLETE", "zombie_slayer_1")

-- Play celebration music
SceneManager:addAction(scene, {
    type = "PLAY_AUDIO",
    stringParam = "quest_complete",
    duration = 0.1
})

-- Level up sound
SceneManager:addAction(scene, {
    type = "PLAY_AUDIO",
    stringParam = "player_level_up",
    duration = 0.1
})

-- Show completion text
SceneManager:addAction(scene, {
    type = "SHOW_TEXT",
    stringParam = "Quest Complete: Zombie Slayer I",
    duration = 3.0
})

-- Wait
SceneManager:addAction(scene, {
    type = "WAIT",
    duration = 1.0
})

-- Elder congratulates
SceneManager:addAction(scene, {
    type = "DIALOG",
    duration = 6.0,
    dialogLines = {
        {
            speakerName = "Village Elder",
            text = "Well done! You've protected our village. Here's your reward.",
            audioFile = "tutorial/elder_congratulate.ogg",
            displayTime = 6.0
        }
    }
})

-- Give rewards
SceneManager:addAction(scene, {
    type = "GIVE_ITEM",
    stringParam = "health_potion",
    intParam = 1,
    duration = 0.5
})

-- Show reward notification
SceneManager:addAction(scene, {
    type = "SHOW_TEXT",
    stringParam = "Received: Health Potion x1 + 100 XP + 50 Gold",
    duration = 4.0
})

print("Loaded scene: zombie_quest_complete")
