namespace CloneMine.ChatServer.Models;

public class ServerConfiguration
{
    public int Port { get; set; } = 25566;
    public int MaxHistory { get; set; } = 100;
    public int MaxMessageLength { get; set; } = 256;
    public string ConfigFile { get; set; } = "server_config.txt";
}
