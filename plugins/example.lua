-- Example plugin for CloneMine
-- This plugin demonstrates the safe plugin API

function onLoad()
    log("Example plugin loaded!")
end

function onUpdate(deltaTime)
    -- Called every frame with deltaTime
    -- Safe operations only
end

function onInput()
    -- Handle input events
end

function onRender()
    -- Called during rendering
end

-- Helper function to demonstrate Lua code
function greet(name)
    return "Hello, " .. name .. "!"
end
