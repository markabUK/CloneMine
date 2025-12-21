namespace CloneMine.QuestServer.Models;

/// <summary>
/// Server configuration settings
/// </summary>
public class ServerConfiguration
{
    public int Port { get; set; } = 25567;
    public string ConfigFile { get; set; } = "server_config.txt";
}
