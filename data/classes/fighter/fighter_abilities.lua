-- Fighter Abilities
return {
    abilities = {
        {name = "Charge", level = 1, resourceCost = 15, cooldown = 15, description = "Rush to target, stun 2s"},
        {name = "Shield Bash", level = 3, resourceCost = 20, cooldown = 12, description = "Stun with shield"},
        {name = "Whirlwind", level = 6, resourceCost = 30, cooldown = 20, description = "AOE melee attack"},
        {name = "Shield Wall", level = 9, resourceCost = 40, cooldown = 120, description = "-40% damage for 12s"},
        {name = "Berserker Rage", level = 12, resourceCost = 0, cooldown = 180, description = "+30% damage, take +15% for 15s"},
        {name = "Cleave", level = 14, resourceCost = 25, cooldown = 8, description = "Hit 2 targets in front"},
        {name = "Intimidating Shout", level = 16, resourceCost = 20, cooldown = 90, description = "Fear nearby enemies 8s"},
        {name = "Last Stand", level = 18, resourceCost = 0, cooldown = 480, description = "+50% max HP for 20s"},
        {name = "Recklessness", level = 19, resourceCost = 0, cooldown = 300, description = "100% crit for 15s, +20% damage taken"},
        {name = "Bladestorm", level = 20, resourceCost = 60, cooldown = 90, description = "Immune, whirlwind for 6s"}
    }
}
