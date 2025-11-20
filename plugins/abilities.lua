-- Character Class Abilities Plugin
-- Defines all abilities for each class with level requirements

-- Ability data structure:
-- {level, name, cost, description, effects}

local ClassAbilities = {
    WIZARD = {
        {level = 1, name = "Magic Missile", cost = 10, damage = 15, 
         description = "Launch magical projectiles at target", castTime = 1.5},
        {level = 3, name = "Fireball", cost = 25, damage = 40, aoe = true, aoeRadius = 8,
         description = "Hurl explosive fire at enemies", castTime = 2.5},
        {level = 5, name = "Ice Storm", cost = 35, damage = 50, aoe = true, aoeRadius = 10, slow = 0.5,
         description = "Call down freezing ice shards", castTime = 3.0},
        {level = 7, name = "Lightning Bolt", cost = 30, damage = 60, chain = 3,
         description = "Strike with lightning that chains to nearby enemies", castTime = 2.0},
        {level = 10, name = "Meteor", cost = 60, damage = 100, aoe = true, aoeRadius = 12,
         description = "Summon a meteor to devastate your foes", castTime = 4.0},
        {level = 13, name = "Polymorph", cost = 40, crowd = "polymorph", duration = 30,
         description = "Transform enemy into a sheep", castTime = 1.8},
        {level = 15, name = "Arcane Blast", cost = 45, damage = 80, stackable = true,
         description = "Powerful arcane spell that gains power with each cast", castTime = 2.2},
        {level = 18, name = "Blizzard", cost = 70, damage = 35, dot = 10, duration = 8, aoe = true, aoeRadius = 15,
         description = "Channel a freezing blizzard", channeled = true},
        {level = 20, name = "Time Warp", cost = 100, buff = "haste", value = 0.3, duration = 40, aoe = true,
         description = "Warp time to increase casting speed for your party", castTime = 0}
    },
    
    SORCERER = {
        {level = 1, name = "Chaos Bolt", cost = 12, damage = 20, random = true,
         description = "Unleash chaotic magic with unpredictable damage", castTime = 2.0},
        {level = 3, name = "Fireball", cost = 25, damage = 40, aoe = true, aoeRadius = 8,
         description = "Hurl explosive fire at enemies", castTime = 2.0},  -- Faster than Wizard
        {level = 5, name = "Arcane Barrage", cost = 30, damage = 45, multiHit = 4,
         description = "Fire multiple arcane missiles", castTime = 1.5},
        {level = 7, name = "Wild Magic Surge", cost = 35, damage = 50, unpredictable = true,
         description = "Tap into wild magic with random effects", castTime = 2.5},
        {level = 10, name = "Chain Lightning", cost = 50, damage = 70, chain = 5,
         description = "Lightning that bounces between enemies", castTime = 2.0},
        {level = 13, name = "Haste", cost = 30, buff = "haste", value = 0.2, duration = 30,
         description = "Increase your casting speed", castTime = 0},
        {level = 15, name = "Meteor Storm", cost = 80, damage = 90, aoe = true, aoeRadius = 20,
         description = "Rain meteors on a large area", castTime = 3.5},
        {level = 18, name = "Time Stop", cost = 100, crowd = "stun", duration = 5, aoe = true,
         description = "Freeze time for all enemies briefly", castTime = 0},
        {level = 20, name = "Wish", cost = 150, versatile = true,
         description = "Ultimate spell that can duplicate any other spell", castTime = 5.0}
    },
    
    PRIEST = {
        {level = 1, name = "Heal", cost = 15, healing = 30,
         description = "Restore health to target ally", castTime = 2.5},
        {level = 3, name = "Holy Smite", cost = 20, damage = 35, undeadBonus = 2.0,
         description = "Smite enemies with holy power, extra damage to undead", castTime = 1.5},
        {level = 5, name = "Divine Shield", cost = 25, shield = 50, duration = 10,
         description = "Protect ally with a divine barrier", castTime = 0},
        {level = 7, name = "Mass Heal", cost = 40, healing = 40, aoe = true, aoeRadius = 15,
         description = "Heal all nearby allies", castTime = 3.0},
        {level = 10, name = "Resurrection", cost = 100, revive = true,
         description = "Return a dead ally to life", castTime = 10.0},
        {level = 13, name = "Holy Nova", cost = 35, healing = 25, damage = 30, aoe = true, aoeRadius = 10,
         description = "Heal allies and damage enemies around you", castTime = 0},
        {level = 15, name = "Prayer of Mending", cost = 50, healing = 40, bounces = 5,
         description = "Healing spell that jumps between injured allies", castTime = 1.5},
        {level = 18, name = "Fear", cost = 30, crowd = "fear", duration = 8,
         description = "Strike fear into enemies' hearts", castTime = 1.8},
        {level = 20, name = "Divine Hymn", cost = 80, healing = 100, hot = 20, duration = 8, aoe = true,
         description = "Channel powerful healing to all allies", channeled = true}
    },
    
    FIGHTER = {
        {level = 1, name = "Power Strike", cost = 15, damage = 25, physicalBonus = 1.5,
         description = "Devastating melee attack", instant = true},
        {level = 3, name = "Whirlwind", cost = 25, damage = 30, aoe = true, aoeRadius = 8,
         description = "Spin and damage all nearby enemies", instant = true},
        {level = 5, name = "Execute", cost = 20, damage = 60, lowHealthBonus = 2.0,
         description = "Execute low health enemies for massive damage", instant = true},
        {level = 7, name = "Battle Cry", cost = 30, buff = "damage", value = 0.25, duration = 15, aoe = true,
         description = "Rally allies with a powerful cry", instant = true},
        {level = 10, name = "Bladestorm", cost = 50, damage = 40, aoe = true, duration = 5,
         description = "Become a whirlwind of blades", channeled = true},
        {level = 13, name = "Charge", cost = 10, damage = 20, stun = 2,
         description = "Charge at enemy and stun them", instant = true},
        {level = 15, name = "Recklessness", cost = 0, buff = "critChance", value = 0.5, duration = 20,
         description = "Increase critical strike chance", instant = true},
        {level = 18, name = "Shield Wall", cost = 40, buff = "defense", value = 0.5, duration = 12,
         description = "Greatly increase your defense", instant = true},
        {level = 20, name = "Avatar", cost = 60, buff = "allCombat", value = 0.4, duration = 20,
         description = "Transform into an avatar of war", instant = true}
    },
    
    RANGER = {
        {level = 1, name = "Aimed Shot", cost = 10, damage = 25, accurate = true,
         description = "Carefully aimed shot with high accuracy", castTime = 2.0},
        {level = 2, name = "Call Pet", cost = 0, summonPet = true,
         description = "Summon your animal companion", castTime = 3.0},
        {level = 3, name = "Multi-Shot", cost = 20, damage = 20, targets = 3,
         description = "Shoot multiple targets at once", instant = true},
        {level = 5, name = "Trap", cost = 15, damage = 35, crowd = "root", duration = 5,
         description = "Place a trap to root enemies", castTime = 1.0},
        {level = 7, name = "Camouflage", cost = 25, stealth = true, duration = 60,
         description = "Blend into surroundings", instant = true},
        {level = 10, name = "Barrage", cost = 40, damage = 50, rapid = true, hits = 10,
         description = "Rapidly fire many arrows", channeled = true},
        {level = 13, name = "Mend Pet", cost = 20, healing = 50, targetPet = true,
         description = "Heal your pet companion", instant = true},
        {level = 15, name = "Aspect of the Hawk", cost = 30, buff = "rangedDamage", value = 0.3, duration = 300,
         description = "Increase ranged attack power", instant = true},
        {level = 18, name = "Bestial Wrath", cost = 50, petBuff = "damage", value = 0.5, duration = 15,
         description = "Send your pet into a rage", instant = true},
        {level = 20, name = "Volley", cost = 70, damage = 60, aoe = true, aoeRadius = 15, duration = 6,
         description = "Rain arrows on an area", channeled = true}
    },
    
    WARLOCK = {
        {level = 1, name = "Shadow Bolt", cost = 10, damage = 20, dot = 5, dotDuration = 6,
         description = "Blast enemy with shadow magic", castTime = 2.5},
        {level = 2, name = "Summon Demon", cost = 20, summonPet = true,
         description = "Summon a demon to fight for you", castTime = 6.0},
        {level = 3, name = "Curse of Weakness", cost = 15, debuff = "damage", value = -0.3, duration = 20,
         description = "Curse enemy to reduce their damage", castTime = 1.0},
        {level = 5, name = "Drain Life", cost = 20, damage = 25, lifesteal = 0.5,
         description = "Drain life from enemy to heal yourself", channeled = true},
        {level = 7, name = "Fear", cost = 25, crowd = "fear", duration = 8,
         description = "Cause enemy to flee in terror", castTime = 1.5},
        {level = 10, name = "Soul Siphon", cost = 50, damage = 70, resourceDrain = 30,
         description = "Drain enemy's soul and resources", castTime = 3.0},
        {level = 13, name = "Hellfire", cost = 40, damage = 45, aoe = true, aoeRadius = 10, selfDamage = 10,
         description = "Rain fire around you, damaging all including yourself", channeled = true},
        {level = 15, name = "Soulstone", cost = 60, revive = true, duration = 300,
         description = "Create a soulstone to revive yourself when killed", castTime = 3.0},
        {level = 18, name = "Chaos Bolt", cost = 60, damage = 100, ignoreArmor = true,
         description = "Ultimate chaos spell that ignores armor", castTime = 2.5},
        {level = 20, name = "Summon Infernal", cost = 80, summonPet = "infernal", duration = 60,
         description = "Summon a powerful infernal demon", castTime = 3.0}
    },
    
    DRUID = {
        {level = 1, name = "Wrath", cost = 10, damage = 18, nature = true,
         description = "Blast of nature's wrath", castTime = 2.0},
        {level = 2, name = "Shapeshift: Bear", cost = 20, shapeshift = "BEAR",
         description = "Transform into bear form", instant = true},
        {level = 3, name = "Healing Touch", cost = 15, healing = 35,
         description = "Channel nature's healing", castTime = 2.5},
        {level = 5, name = "Moonfire", cost = 20, damage = 25, dot = 10, dotDuration = 12,
         description = "Call down lunar wrath", instant = true},
        {level = 7, name = "Shapeshift: Cat", cost = 20, shapeshift = "CAT",
         description = "Transform into cat form", instant = true},
        {level = 10, name = "Starfall", cost = 50, damage = 60, aoe = true, aoeRadius = 20, duration = 10,
         description = "Stars rain down on enemies", channeled = true},
        {level = 13, name = "Rejuvenation", cost = 25, hot = 10, duration = 15,
         description = "Heal target over time", instant = true},
        {level = 15, name = "Shapeshift: Lion", cost = 30, shapeshift = "LION",
         description = "Transform into lion form", instant = true},
        {level = 18, name = "Tranquility", cost = 80, healing = 50, hot = 15, duration = 8, aoe = true,
         description = "Channel peaceful healing to all allies", channeled = true},
        {level = 20, name = "Force of Nature", cost = 70, summon = "treants", count = 3, duration = 30,
         description = "Summon treants to fight for you", castTime = 1.5}
    },
    
    MASTERMIND = {
        {level = 1, name = "Summon Minion", cost = 20, summonPet = "minion",
         description = "Summon a basic minion pet", castTime = 3.0},
        {level = 1, name = "Soothe", cost = 10, healing = 15, hot = 5, duration = 9,
         description = "Soothing heal over time", castTime = 1.5},
        {level = 3, name = "Fortify", cost = 20, buff = "defense", value = 0.25, duration = 60,
         description = "Increase target's defenses", castTime = 1.0},
        {level = 5, name = "Rally", cost = 30, healing = 25, buff = "speed", value = 0.2, aoe = true, aoeRadius = 20,
         description = "Rally and heal nearby allies", instant = true},
        {level = 7, name = "Empower Pets", cost = 25, petDamageBonus = 2.0, duration = 15,
         description = "Double your pets' damage", instant = true},
        {level = 10, name = "Summon Lieutenant", cost = 40, summonPet = "lieutenant",
         description = "Summon a stronger lieutenant pet", castTime = 4.0},
        {level = 13, name = "Mass Fortification", cost = 50, buff = "allStats", value = 0.15, duration = 60, aoe = true,
         description = "Increase all stats for nearby allies", castTime = 2.0},
        {level = 15, name = "Tactical Supremacy", cost = 35, buff = "accuracy", value = 0.3, duration = 30, aoe = true,
         description = "Increase hit chance for party", instant = true},
        {level = 18, name = "Overwhelming Force", cost = 60, petAbility = "massAttack",
         description = "Command all pets to attack simultaneously", instant = true},
        {level = 20, name = "Summon Boss Pet", cost = 60, summonPet = "boss",
         description = "Summon a powerful boss-level pet", castTime = 5.0}
    }
}

-- Shared abilities (multiple classes can use)
local SharedAbilities = {
    Fireball = {"WIZARD", "SORCERER"},
    Heal = {"PRIEST", "DRUID"},
    Fear = {"WARLOCK", "PRIEST"}
}

-- Helper function to get all abilities for a class
function getAbilitiesForClass(className)
    return ClassAbilities[className] or {}
end

-- Helper function to get abilities available at a specific level
function getAvailableAbilities(className, level)
    local classAbilities = ClassAbilities[className] or {}
    local available = {}
    
    for _, ability in ipairs(classAbilities) do
        if ability.level <= level then
            table.insert(available, ability)
        end
    end
    
    return available
end

-- Helper function to check if ability is shared
function isSharedAbility(abilityName)
    return SharedAbilities[abilityName] ~= nil
end

-- Helper function to get classes that can use an ability
function getClassesForAbility(abilityName)
    return SharedAbilities[abilityName] or {}
end

-- Export
return {
    ClassAbilities = ClassAbilities,
    SharedAbilities = SharedAbilities,
    getAbilitiesForClass = getAbilitiesForClass,
    getAvailableAbilities = getAvailableAbilities,
    isSharedAbility = isSharedAbility,
    getClassesForAbility = getClassesForAbility
}
