namespace CloneMine.GameServer.Models;

/// <summary>
/// Server configuration settings
/// </summary>
public class ServerConfiguration
{
    public int Port { get; set; } = 25565;
    public int TickRate { get; set; } = 60;
    public float TickInterval => 1.0f / TickRate;
    public string ConfigFile { get; set; } = "server_config.txt";
}
