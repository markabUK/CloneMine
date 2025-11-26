-- Example scripted scene: Epic Wizard Duel
-- Demonstrates chained combat actions, spell casting, and special death effects
-- Triggered when player talks to the Evil Wizard NPC
-- Note: Replace "PlayerCharacterName" with the actual character name

local scene = SceneManager:createScene("wizard_duel")

-- Trigger on NPC interaction
SceneManager:addTrigger(scene, "NPC_INTERACTION", "evil_wizard")

-- Dialog: Wizard challenges the player
SceneManager:addAction(scene, {
    type = "DIALOG",
    duration = 4.0,
    dialogLines = {{
        speakerName = "Evil Wizard",
        text = "You dare challenge me? Face my power!",
        audioFile = "dialog/wizard_challenge.ogg"
    }}
})

-- Wait a moment
SceneManager:addAction(scene, {
    type = "WAIT",
    duration = 1.0
})

-- Wizard casts fireball at player (use actual character name)
SceneManager:addAction(scene, {
    type = "CAST_SPELL",
    stringParam = "evil_wizard",      -- caster entity ID
    stringParam2 = "fireball",        -- spell name
    vec3Param = {x = 0, y = 0, z = 0}, -- target position (player)
    duration = 2.0
})

-- Play fireball sound
SceneManager:addAction(scene, {
    type = "PLAY_AUDIO",
    stringParam = "fireball_cast",
    duration = 0.1
})

-- Wait for fireball to travel
SceneManager:addAction(scene, {
    type = "WAIT",
    duration = 1.0
})

-- Player takes damage (use actual character name)
SceneManager:addAction(scene, {
    type = "TAKE_DAMAGE",
    stringParam = "PlayerCharacterName",  -- Replace with actual character name
    stringParam2 = "fire",
    intParam = 50,  -- damage amount
    duration = 0.5
})

-- Play impact sound
SceneManager:addAction(scene, {
    type = "PLAY_AUDIO",
    stringParam = "fireball_impact",
    duration = 0.1
})

-- Dramatic pause
SceneManager:addAction(scene, {
    type = "WAIT",
    duration = 1.5
})

-- Player retaliates with attack (use actual character name)
SceneManager:addAction(scene, {
    type = "ATTACK",
    stringParam = "PlayerCharacterName",  -- Replace with actual character name
    stringParam2 = "evil_wizard",
    duration = 1.0
})

-- Play sword hit sound
SceneManager:addAction(scene, {
    type = "PLAY_AUDIO",
    stringParam = "sword_hit",
    duration = 0.1
})

-- Wizard takes damage
SceneManager:addAction(scene, {
    type = "TAKE_DAMAGE",
    stringParam = "evil_wizard",
    stringParam2 = "physical",
    intParam = 30,
    duration = 0.5
})

-- Wizard gets angry, casts more powerful spell
SceneManager:addAction(scene, {
    type = "DIALOG",
    duration = 3.0,
    dialogLines = {{
        speakerName = "Evil Wizard",
        text = "You will pay for that! Witness TRUE power!",
        audioFile = "dialog/wizard_angry.ogg"
    }}
})

-- Wizard casts disintegration spell
SceneManager:addAction(scene, {
    type = "CAST_SPELL",
    stringParam = "evil_wizard",
    stringParam2 = "disintegration",
    vec3Param = {x = -5, y = 0, z = -5},  -- targets a nearby weak enemy
    duration = 3.0
})

-- Play powerful spell sound
SceneManager:addAction(scene, {
    type = "PLAY_AUDIO",
    stringParam = "disintegration_cast",
    duration = 0.1
})

-- Wait for spell to take effect
SceneManager:addAction(scene, {
    type = "WAIT",
    duration = 1.5
})

-- Create area effect at target
SceneManager:addAction(scene, {
    type = "AREA_EFFECT",
    stringParam = "disintegration",  -- effect type
    vec3Param = {x = -5, y = 0, z = -5},
    floatParam = 3.0,  -- radius
    intParam = 200,    -- power
    duration = 2.0
})

-- Play disintegration sound
SceneManager:addAction(scene, {
    type = "PLAY_AUDIO",
    stringParam = "disintegration_effect",
    duration = 0.1
})

-- Weak enemy gets vaporized
SceneManager:addAction(scene, {
    type = "DESTROY_ENTITY",
    stringParam = "weak_goblin",
    stringParam2 = "vaporize",  -- destruction type
    duration = 1.0
})

-- Show text about the effect
SceneManager:addAction(scene, {
    type = "SHOW_TEXT",
    stringParam = "The goblin is completely vaporized!",
    duration = 3.0
})

-- Player realizes they need to finish this quickly
SceneManager:addAction(scene, {
    type = "WAIT",
    duration = 1.0
})

-- Player uses special ability (use actual character name)
SceneManager:addAction(scene, {
    type = "USE_ABILITY",
    stringParam = "PlayerCharacterName",  -- Replace with actual character name
    stringParam2 = "berserk_rage",
    duration = 1.5
})

-- Apply buff to player (use actual character name)
SceneManager:addAction(scene, {
    type = "APPLY_STATUS",
    stringParam = "PlayerCharacterName",  -- Replace with actual character name
    stringParam2 = "berserk",
    floatParam = 10.0,  -- status duration
    duration = 0.5
})

-- Chain of attacks while buffed (use actual character name)
SceneManager:addAction(scene, {
    type = "ATTACK",
    stringParam = "PlayerCharacterName",  -- Replace with actual character name
    stringParam2 = "evil_wizard",
    duration = 0.5
})

SceneManager:addAction(scene, {
    type = "ATTACK",
    stringParam = "PlayerCharacterName",  -- Replace with actual character name
    stringParam2 = "evil_wizard",
    duration = 0.5
})

SceneManager:addAction(scene, {
    type = "ATTACK",
    stringParam = "PlayerCharacterName",  -- Replace with actual character name
    stringParam2 = "evil_wizard",
    duration = 0.5
})

-- Play combo attack sounds
SceneManager:addAction(scene, {
    type = "PLAY_AUDIO",
    stringParam = "sword_combo",
    duration = 0.1
})

-- Wizard takes heavy damage
SceneManager:addAction(scene, {
    type = "TAKE_DAMAGE",
    stringParam = "evil_wizard",
    stringParam2 = "physical",
    intParam = 150,
    duration = 0.5
})

-- Wizard's final words
SceneManager:addAction(scene, {
    type = "DIALOG",
    duration = 3.0,
    dialogLines = {{
        speakerName = "Evil Wizard",
        text = "No... this cannot be...",
        audioFile = "dialog/wizard_defeat.ogg"
    }}
})

-- Wizard dies
SceneManager:addAction(scene, {
    type = "DIE",
    stringParam = "evil_wizard",
    stringParam2 = "explode",  -- death type
    duration = 2.0
})

-- Play death sound
SceneManager:addAction(scene, {
    type = "PLAY_AUDIO",
    stringParam = "wizard_death",
    duration = 0.1
})

-- Explosion area effect from wizard's death
SceneManager:addAction(scene, {
    type = "AREA_EFFECT",
    stringParam = "explosion",
    vec3Param = {x = 10, y = 0, z = 10},  -- wizard's position
    floatParam = 5.0,  -- radius
    intParam = 50,     -- knockback power
    duration = 1.0
})

-- Show victory text
SceneManager:addAction(scene, {
    type = "SHOW_TEXT",
    stringParam = "The Evil Wizard has been defeated!",
    duration = 4.0
})

-- Remove berserk status (use actual character name)
SceneManager:addAction(scene, {
    type = "REMOVE_STATUS",
    stringParam = "PlayerCharacterName",  -- Replace with actual character name
    stringParam2 = "berserk",
    duration = 0.5
})

-- Reward player
SceneManager:addAction(scene, {
    type = "GIVE_ITEM",
    stringParam = "wizard_staff",
    intParam = 1,
    duration = 0.1
})

-- Complete associated quest
SceneManager:addAction(scene, {
    type = "COMPLETE_QUEST",
    stringParam = "defeat_evil_wizard",
    duration = 0.1
})

-- Final message
SceneManager:addAction(scene, {
    type = "SHOW_TEXT",
    stringParam = "Quest Complete: The Evil Wizard has been vanquished!",
    duration = 4.0
})

print("Loaded scene: wizard_duel")
