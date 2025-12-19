using CloneMine.AuctionServer.Models;

namespace CloneMine.AuctionServer.Interfaces;

/// <summary>
/// Interface for auction data persistence
/// Single Responsibility: Auction storage
/// </summary>
public interface IAuctionRepository
{
    Task<AuctionItem?> GetAuctionAsync(int auctionId);
    Task<IEnumerable<AuctionItem>> GetActiveAuctionsAsync();
    Task<IEnumerable<AuctionItem>> GetUserAuctionsAsync(string username);
    Task<int> CreateAuctionAsync(AuctionItem auction);
    Task<bool> UpdateAuctionAsync(AuctionItem auction);
    Task<bool> DeleteAuctionAsync(int auctionId);
}
