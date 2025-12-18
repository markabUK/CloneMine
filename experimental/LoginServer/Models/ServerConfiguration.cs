namespace CloneMine.LoginServer.Models;

/// <summary>
/// Configuration settings for the login server
/// </summary>
public class ServerConfiguration
{
    public int Port { get; set; } = 25564;
    public int MaxCharactersPerAccount { get; set; } = 5;
    public string ConfigFile { get; set; } = "server_config.txt";
    public int MinUsernameLength { get; set; } = 3;
    public int MaxUsernameLength { get; set; } = 16;
    public int MinPasswordLength { get; set; } = 6;
    public int MaxPasswordLength { get; set; } = 64;
}
