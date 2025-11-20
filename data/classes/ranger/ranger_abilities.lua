-- Ranger Abilities

return {
    abilities = {
        {name = "Steady Shot", level = 1, resourceCost = 10, cooldown = 0, castTime = 2, description = "Basic ranged attack"},
        {name = "Multi-Shot", level = 6, resourceCost = 25, cooldown = 10, castTime = 0, description = "Hits 3 targets"},
        {name = "Rapid Fire", level = 10, resourceCost = 40, cooldown = 180, castTime = 0, description = "Increases attack speed 40% for 15s"},
        {name = "Feign Death", level = 12, resourceCost = 0, cooldown = 30, castTime = 0, description = "Pretend to be dead"},
        {name = "Aimed Shot", level = 14, resourceCost = 50, cooldown = 12, castTime = 3, description = "Powerful ranged attack"},
        {name = "Disengage", level = 16, resourceCost = 20, cooldown = 20, castTime = 0, description = "Leap backward 15 yards"},
        {name = "Tranquilizing Shot", level = 18, resourceCost = 30, cooldown = 10, castTime = 0, description = "Removes enrage effects"},
        {name = "Kill Command", level = 20, resourceCost = 60, cooldown = 60, castTime = 0, description = "Command pet to attack"}
    }
}
