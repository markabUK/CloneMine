namespace CloneMine.CharacterServer.Models;

/// <summary>
/// Server configuration settings
/// </summary>
public class ServerConfiguration
{
    public int Port { get; set; } = 25568;
    public int MaxCharactersPerAccount { get; set; } = 5;
    public string ConfigFile { get; set; } = "server_config.txt";
    public int MinCharacterNameLength { get; set; } = 3;
    public int MaxCharacterNameLength { get; set; } = 16;
}
