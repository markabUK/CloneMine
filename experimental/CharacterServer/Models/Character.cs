namespace CloneMine.CharacterServer.Models;

/// <summary>
/// Represents a player character
/// </summary>
public class Character
{
    public string Name { get; set; } = string.Empty;
    public string Owner { get; set; } = string.Empty;
    public int Level { get; set; }
    public int Experience { get; set; }
    public float Health { get; set; }
    public float MaxHealth { get; set; }
    public float PositionX { get; set; }
    public float PositionY { get; set; }
    public float PositionZ { get; set; }
}
