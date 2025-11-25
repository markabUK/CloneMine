-- Example scripted scene: Tutorial Quest Start
-- This scene is triggered when the player starts the tutorial quest

local scene = SceneManager:createScene("tutorial_quest_start")

-- Set up trigger
SceneManager:addTrigger(scene, "QUEST_START", "tutorial_quest")

-- Fade in
SceneManager:addAction(scene, {
    type = "FADE_IN",
    duration = 2.0
})

-- Play ambient music
SceneManager:addAction(scene, {
    type = "PLAY_AUDIO",
    stringParam = "tutorial_music",
    duration = 0.1
})

-- Wait a moment
SceneManager:addAction(scene, {
    type = "WAIT",
    duration = 1.0
})

-- Elder speaks
SceneManager:addAction(scene, {
    type = "DIALOG",
    duration = 5.0,
    dialogLines = {
        {
            speakerName = "Village Elder",
            text = "Welcome, young adventurer! I have a quest for you.",
            audioFile = "tutorial/elder_welcome.ogg",
            displayTime = 5.0
        }
    }
})

-- Player responds
SceneManager:addAction(scene, {
    type = "DIALOG",
    duration = 4.0,
    dialogLines = {
        {
            speakerName = "Player",
            text = "I'm ready to help!",
            audioFile = "tutorial/player_ready.ogg",
            displayTime = 4.0
        }
    }
})

-- Elder explains
SceneManager:addAction(scene, {
    type = "DIALOG",
    duration = 7.0,
    dialogLines = {
        {
            speakerName = "Village Elder",
            text = "Zombies have been spotted near the village. Please defeat 10 of them.",
            audioFile = "tutorial/elder_quest.ogg",
            displayTime = 7.0
        }
    }
})

-- Quest accept sound
SceneManager:addAction(scene, {
    type = "PLAY_AUDIO",
    stringParam = "quest_accept",
    duration = 0.1
})

-- Show quest text
SceneManager:addAction(scene, {
    type = "SHOW_TEXT",
    stringParam = "New Quest: Zombie Slayer I",
    duration = 3.0
})

print("Loaded scene: tutorial_quest_start")
