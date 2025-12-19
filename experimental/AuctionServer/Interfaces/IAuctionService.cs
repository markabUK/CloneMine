using CloneMine.AuctionServer.Models;

namespace CloneMine.AuctionServer.Interfaces;

/// <summary>
/// Interface for auction business logic
/// Single Responsibility: Auction management
/// </summary>
public interface IAuctionService
{
    Task<(bool Success, string Message, int AuctionId)> CreateAuctionAsync(string seller, string itemName, int quantity, int startingPrice);
    Task<(bool Success, string Message)> PlaceBidAsync(int auctionId, string bidder, int amount);
    Task<(bool Success, string Message)> CancelAuctionAsync(int auctionId, string seller);
    Task<IEnumerable<AuctionItem>> GetActiveAuctionsAsync();
    Task<IEnumerable<AuctionItem>> GetUserAuctionsAsync(string username);
    Task<AuctionItem?> GetAuctionAsync(int auctionId);
    Task CheckExpirationsAsync();
}
