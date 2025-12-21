using System.Collections.Concurrent;
using CloneMine.AuctionServer.Interfaces;
using CloneMine.AuctionServer.Models;

namespace CloneMine.AuctionServer.Repositories;

/// <summary>
/// In-memory auction repository
/// Single Responsibility: Auction data storage
/// </summary>
public class InMemoryAuctionRepository : IAuctionRepository
{
    private readonly ConcurrentDictionary<int, AuctionItem> _auctions;
    private int _nextId = 1;

    public InMemoryAuctionRepository()
    {
        _auctions = new ConcurrentDictionary<int, AuctionItem>();
    }

    public Task<AuctionItem?> GetAuctionAsync(int auctionId)
    {
        _auctions.TryGetValue(auctionId, out var auction);
        return Task.FromResult(auction);
    }

    public Task<IEnumerable<AuctionItem>> GetActiveAuctionsAsync()
    {
        var active = _auctions.Values.Where(a => a.IsActive).ToList();
        return Task.FromResult<IEnumerable<AuctionItem>>(active);
    }

    public Task<IEnumerable<AuctionItem>> GetUserAuctionsAsync(string username)
    {
        if (string.IsNullOrWhiteSpace(username))
        {
            return Task.FromResult<IEnumerable<AuctionItem>>(Array.Empty<AuctionItem>());
        }

        var userAuctions = _auctions.Values
            .Where(a => a.Seller.Equals(username, StringComparison.OrdinalIgnoreCase) && a.IsActive)
            .ToList();
        return Task.FromResult<IEnumerable<AuctionItem>>(userAuctions);
    }

    public Task<int> CreateAuctionAsync(AuctionItem auction)
    {
        if (auction == null)
        {
            throw new ArgumentNullException(nameof(auction));
        }

        auction.AuctionId = _nextId++;
        _auctions[auction.AuctionId] = auction;
        return Task.FromResult(auction.AuctionId);
    }

    public Task<bool> UpdateAuctionAsync(AuctionItem auction)
    {
        if (auction == null)
        {
            throw new ArgumentNullException(nameof(auction));
        }

        _auctions[auction.AuctionId] = auction;
        return Task.FromResult(true);
    }

    public Task<bool> DeleteAuctionAsync(int auctionId)
    {
        return Task.FromResult(_auctions.TryRemove(auctionId, out _));
    }
}
