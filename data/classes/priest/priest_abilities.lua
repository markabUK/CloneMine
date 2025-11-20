-- Priest Abilities
return {
    abilities = {
        {name = "Renew", level = 1, resourceCost = 15, cooldown = 0, description = "Heal over time"},
        {name = "Flash Heal", level = 4, resourceCost = 35, cooldown = 0, description = "Quick heal"},
        {name = "Prayer of Mending", level = 8, resourceCost = 40, cooldown = 10, description = "Bouncing heal"},
        {name = "Power Infusion", level = 12, resourceCost = 0, cooldown = 120, description = "+25% haste for 20s"},
        {name = "Pain Suppression", level = 14, resourceCost = 50, cooldown = 180, description = "Reduce damage 40% for 8s"},
        {name = "Divine Star", level = 16, resourceCost = 30, cooldown = 15, description = "AOE heal/damage"},
        {name = "Apotheosis", level = 20, resourceCost = 0, cooldown = 300, description = "Reset all spell cooldowns"}
    }
}
