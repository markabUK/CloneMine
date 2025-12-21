using System.Numerics;

namespace CloneMine.GameServer.Models;

/// <summary>
/// Represents a player in the game world
/// </summary>
public class Player
{
    public string Username { get; set; } = string.Empty;
    public Vector3 Position { get; set; }
    public Vector3 Velocity { get; set; }
    public float Health { get; set; }
    public float MaxHealth { get; set; }
    public int Level { get; set; }
}
