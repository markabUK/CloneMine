-- Wizard Spells
-- Arcane magic learned through study

return {
    -- Spell list (references spells from data/spells/player_spells.lua)
    spells = {
        "Arcane Missiles",
        "Fireball",
        "Frost Bolt",
        "Polymorph",
        "Arcane Explosion",
        "Ice Block",
        "Blizzard",
        "Meteor",
        "Arcane Blast",
        "Slow",
        "Invisibility",
        "Counterspell",
        "Time Warp",
        "Arcane Brilliance",
        "Teleport"
    },
    
    -- Spell unlock levels
    spellUnlocks = {
        [1] = {"Arcane Missiles", "Frost Bolt"},
        [3] = {"Fireball"},
        [5] = {"Polymorph", "Slow"},
        [7] = {"Arcane Explosion"},
        [9] = {"Ice Block", "Counterspell"},
        [11] = {"Blizzard"},
        [13] = {"Arcane Blast", "Invisibility"},
        [15] = {"Meteor"},
        [17] = {"Time Warp"},
        [19] = {"Arcane Brilliance", "Teleport"}
    }
}
