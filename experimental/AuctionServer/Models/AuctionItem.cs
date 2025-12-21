namespace CloneMine.AuctionServer.Models;

/// <summary>
/// Represents an auction item
/// </summary>
public class AuctionItem
{
    public int AuctionId { get; set; }
    public string Seller { get; set; } = string.Empty;
    public string ItemName { get; set; } = string.Empty;
    public int Quantity { get; set; }
    public int StartingPrice { get; set; }
    public int CurrentBid { get; set; }
    public string? CurrentBidder { get; set; }
    public DateTime ExpirationTime { get; set; }
    public bool IsActive { get; set; }
}
