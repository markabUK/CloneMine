namespace CloneMine.AuctionServer.Models;

/// <summary>
/// Represents a bid on an auction
/// </summary>
public class Bid
{
    public string Bidder { get; set; } = string.Empty;
    public int Amount { get; set; }
    public DateTime BidTime { get; set; }
}
