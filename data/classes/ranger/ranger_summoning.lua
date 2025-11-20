-- Ranger Summoning System
-- Animal companion mechanics

return {
    maxSummons = 1,
    
    summonTiers = {
        {
            tier = 1,
            name = "Animal Companion",
            maxCount = 1,
            unlockLevel = 1
        }
    },
    
    -- Pet selection (player chooses one at level 1)
    petChoices = {"WOLF", "BEAR", "HAWK"},
    
    -- Taming mechanics
    tamingSystem = {
        enabled = true,
        maxTameLevel = 20,
        tameDuration = 10,  -- seconds
        requiresChannel = true
    }
}
