-- Class Definitions: Character class archetypes and their powerset rules
-- Loaded dynamically by the ClassSystem

return {
    -- =========================================================================
    -- ATTACKER CLASSES - Primary: Attack, Secondary: Attack or Heal
    -- =========================================================================
    {
        name = "Mage",
        description = "Masters of arcane and elemental magic, capable of devastating ranged attacks",
        archetype = "ATTACKER",
        primaryStat = "intelligence",
        resourceType = "mana",
        baseHealth = 80.0,
        healthPerLevel = 8.0,
        baseResource = 120.0,
        resourcePerLevel = 8.0,
        secondaryEffectiveness = 0.80,
        -- Which powerset categories this class can choose for primary
        allowedPrimaryCategories = { "ATTACK" },
        -- Which powerset categories this class can choose for secondary
        allowedSecondaryCategories = { "ATTACK", "HEALING" },
        -- Specific powersets this class has access to (by name)
        allowedPrimaryPowersets = { "Fire Blast", "Ice Assault", "Lightning Strike", "Arcane Power" },
        allowedSecondaryPowersets = { "Fire Blast", "Ice Assault", "Arcane Power", "Holy Blessing", "Radiant Restoration" }
    },
    
    {
        name = "Warlock",
        description = "Dark spellcasters who wield shadow and necrotic magic with destructive power",
        archetype = "ATTACKER",
        primaryStat = "intelligence",
        resourceType = "mana",
        baseHealth = 85.0,
        healthPerLevel = 8.0,
        baseResource = 100.0,
        resourcePerLevel = 7.0,
        secondaryEffectiveness = 0.80,
        allowedPrimaryCategories = { "ATTACK" },
        allowedSecondaryCategories = { "ATTACK", "CONTROL" },
        allowedPrimaryPowersets = { "Shadow Strike", "Necrotic Touch", "Fire Blast" },
        allowedSecondaryPowersets = { "Shadow Strike", "Necrotic Touch", "Mind Control", "Shadow Binding" }
    },
    
    {
        name = "Psion",
        description = "Mental warriors who devastate foes with psionic power",
        archetype = "ATTACKER",
        primaryStat = "wisdom",
        resourceType = "focus",
        baseHealth = 75.0,
        healthPerLevel = 7.0,
        baseResource = 100.0,
        resourcePerLevel = 6.0,
        secondaryEffectiveness = 0.80,
        allowedPrimaryCategories = { "ATTACK" },
        allowedSecondaryCategories = { "ATTACK", "CONTROL" },
        allowedPrimaryPowersets = { "Psionic Assault" },
        allowedSecondaryPowersets = { "Psionic Assault", "Mind Control", "Shadow Strike" }
    },
    
    {
        name = "Elementalist",
        description = "Channel the primal forces of nature in devastating elemental attacks",
        archetype = "ATTACKER",
        primaryStat = "intelligence",
        resourceType = "mana",
        baseHealth = 80.0,
        healthPerLevel = 8.0,
        baseResource = 110.0,
        resourcePerLevel = 7.0,
        secondaryEffectiveness = 0.80,
        allowedPrimaryCategories = { "ATTACK" },
        allowedSecondaryCategories = { "ATTACK", "CONTROL" },
        allowedPrimaryPowersets = { "Fire Blast", "Ice Assault", "Lightning Strike", "Water Torrent" },
        allowedSecondaryPowersets = { "Fire Blast", "Ice Assault", "Ice Control", "Fire Control" }
    },
    
    -- =========================================================================
    -- TANK CLASSES - Primary: Defense, Secondary: Attack
    -- =========================================================================
    {
        name = "Guardian",
        description = "Heavily armored protectors who defend allies and draw enemy fire",
        archetype = "TANK",
        primaryStat = "constitution",
        resourceType = "stamina",
        baseHealth = 150.0,
        healthPerLevel = 15.0,
        baseResource = 80.0,
        resourcePerLevel = 4.0,
        secondaryEffectiveness = 0.70,
        allowedPrimaryCategories = { "DEFENSE" },
        allowedSecondaryCategories = { "ATTACK" },
        allowedPrimaryPowersets = { "Stone Skin", "Radiant Shield", "Holy Bulwark", "Willpower" },
        allowedSecondaryPowersets = { "Slashing Fury", "Crushing Blows", "Holy Smite" }
    },
    
    {
        name = "Frost Knight",
        description = "Ice-armored warriors who combine icy defense with frost attacks",
        archetype = "TANK",
        primaryStat = "constitution",
        resourceType = "runic_power",
        baseHealth = 140.0,
        healthPerLevel = 14.0,
        baseResource = 100.0,
        resourcePerLevel = 5.0,
        secondaryEffectiveness = 0.70,
        allowedPrimaryCategories = { "DEFENSE" },
        allowedSecondaryCategories = { "ATTACK" },
        allowedPrimaryPowersets = { "Ice Armor" },
        allowedSecondaryPowersets = { "Ice Assault", "Slashing Fury", "Piercing Strikes" }
    },
    
    {
        name = "Shadow Knight",
        description = "Dark warriors who use shadow magic to protect themselves",
        archetype = "TANK",
        primaryStat = "constitution",
        resourceType = "blood",
        baseHealth = 135.0,
        healthPerLevel = 13.0,
        baseResource = 100.0,
        resourcePerLevel = 5.0,
        secondaryEffectiveness = 0.70,
        allowedPrimaryCategories = { "DEFENSE" },
        allowedSecondaryCategories = { "ATTACK" },
        allowedPrimaryPowersets = { "Dark Regeneration" },
        allowedSecondaryPowersets = { "Shadow Strike", "Necrotic Touch", "Slashing Fury" }
    },
    
    -- =========================================================================
    -- HEALER CLASSES - Primary: Healing, Secondary: Attack (reduced effectiveness)
    -- =========================================================================
    {
        name = "Cleric",
        description = "Divine healers who channel holy power to restore allies",
        archetype = "HEALER",
        primaryStat = "wisdom",
        resourceType = "mana",
        baseHealth = 90.0,
        healthPerLevel = 9.0,
        baseResource = 120.0,
        resourcePerLevel = 8.0,
        secondaryEffectiveness = 0.65,
        allowedPrimaryCategories = { "HEALING" },
        allowedSecondaryCategories = { "ATTACK" },
        allowedPrimaryPowersets = { "Holy Blessing", "Radiant Restoration" },
        allowedSecondaryPowersets = { "Holy Smite", "Fire Blast" }
    },
    
    {
        name = "Druid",
        description = "Nature healers who draw on primal forces to heal and protect",
        archetype = "HEALER",
        primaryStat = "wisdom",
        resourceType = "mana",
        baseHealth = 95.0,
        healthPerLevel = 9.0,
        baseResource = 110.0,
        resourcePerLevel = 7.0,
        secondaryEffectiveness = 0.65,
        allowedPrimaryCategories = { "HEALING" },
        allowedSecondaryCategories = { "ATTACK" },
        allowedPrimaryPowersets = { "Nature's Embrace" },
        allowedSecondaryPowersets = { "Nature's Wrath", "Lightning Strike" }
    },
    
    {
        name = "Water Shaman",
        description = "Healers who harness the restorative power of water",
        archetype = "HEALER",
        primaryStat = "wisdom",
        resourceType = "mana",
        baseHealth = 90.0,
        healthPerLevel = 9.0,
        baseResource = 115.0,
        resourcePerLevel = 7.0,
        secondaryEffectiveness = 0.65,
        allowedPrimaryCategories = { "HEALING" },
        allowedSecondaryCategories = { "ATTACK" },
        allowedPrimaryPowersets = { "Water Healing" },
        allowedSecondaryPowersets = { "Water Torrent", "Lightning Strike" }
    },
    
    -- =========================================================================
    -- MASTERMIND CLASSES - Primary: Summon, Secondary: Buff/Heal
    -- =========================================================================
    {
        name = "Necromancer",
        description = "Masters of undeath who raise armies of skeletal minions",
        archetype = "MASTERMIND",
        primaryStat = "intelligence",
        resourceType = "mana",
        baseHealth = 70.0,
        healthPerLevel = 7.0,
        baseResource = 100.0,
        resourcePerLevel = 6.0,
        secondaryEffectiveness = 0.75,
        allowedPrimaryCategories = { "SUMMON" },
        allowedSecondaryCategories = { "BUFF", "HEALING" },
        allowedPrimaryPowersets = { "Necromancy" },
        allowedSecondaryPowersets = { "Shadow Empowerment", "Holy Blessing" }
    },
    
    {
        name = "Conjurer",
        description = "Arcane summoners who call forth magical constructs and elementals",
        archetype = "MASTERMIND",
        primaryStat = "intelligence",
        resourceType = "mana",
        baseHealth = 75.0,
        healthPerLevel = 7.0,
        baseResource = 110.0,
        resourcePerLevel = 7.0,
        secondaryEffectiveness = 0.75,
        allowedPrimaryCategories = { "SUMMON" },
        allowedSecondaryCategories = { "BUFF" },
        allowedPrimaryPowersets = { "Conjuration", "Elemental Summoning" },
        allowedSecondaryPowersets = { "Arcane Empowerment", "Psionic Boost" }
    },
    
    {
        name = "Beast Lord",
        description = "Nature masters who command packs of wild beasts",
        archetype = "MASTERMIND",
        primaryStat = "wisdom",
        resourceType = "focus",
        baseHealth = 85.0,
        healthPerLevel = 8.0,
        baseResource = 100.0,
        resourcePerLevel = 5.0,
        secondaryEffectiveness = 0.75,
        allowedPrimaryCategories = { "SUMMON" },
        allowedSecondaryCategories = { "BUFF", "HEALING" },
        allowedPrimaryPowersets = { "Beast Mastery" },
        allowedSecondaryPowersets = { "Nature's Gift", "Nature's Embrace" }
    },
    
    {
        name = "Demonologist",
        description = "Dark summoners who call forth demons from the nether",
        archetype = "MASTERMIND",
        primaryStat = "intelligence",
        resourceType = "mana",
        baseHealth = 70.0,
        healthPerLevel = 7.0,
        baseResource = 100.0,
        resourcePerLevel = 6.0,
        secondaryEffectiveness = 0.75,
        allowedPrimaryCategories = { "SUMMON" },
        allowedSecondaryCategories = { "BUFF" },
        allowedPrimaryPowersets = { "Demon Summoning" },
        allowedSecondaryPowersets = { "Shadow Empowerment", "Arcane Empowerment" }
    }
}
