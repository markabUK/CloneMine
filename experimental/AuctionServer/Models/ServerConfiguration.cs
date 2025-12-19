namespace CloneMine.AuctionServer.Models;

/// <summary>
/// Server configuration settings
/// </summary>
public class ServerConfiguration
{
    public int Port { get; set; } = 25569;
    public string ConfigFile { get; set; } = "server_config.txt";
    public int DefaultAuctionDurationHours { get; set; } = 24;
    public int MaxItemNameLength { get; set; } = 64;
    public int MinBidIncrement { get; set; } = 1;
}
