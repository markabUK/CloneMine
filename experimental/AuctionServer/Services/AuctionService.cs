using CloneMine.AuctionServer.Interfaces;
using CloneMine.AuctionServer.Models;

namespace CloneMine.AuctionServer.Services;

/// <summary>
/// Auction service implementation
/// Single Responsibility: Auction business logic
/// </summary>
public class AuctionService : IAuctionService
{
    private readonly IAuctionRepository _repository;
    private readonly IInputValidator _validator;
    private readonly ServerConfiguration _config;
    private int _totalProcessed = 0;

    public AuctionService(
        IAuctionRepository repository,
        IInputValidator validator,
        ServerConfiguration config)
    {
        _repository = repository ?? throw new ArgumentNullException(nameof(repository));
        _validator = validator ?? throw new ArgumentNullException(nameof(validator));
        _config = config ?? throw new ArgumentNullException(nameof(config));
    }

    public async Task<(bool Success, string Message, int AuctionId)> CreateAuctionAsync(
        string seller, string itemName, int quantity, int startingPrice)
    {
        // Validate inputs
        var sellerValidation = _validator.ValidateUsername(seller);
        if (!sellerValidation.IsValid)
        {
            return (false, sellerValidation.ErrorMessage, 0);
        }

        var itemValidation = _validator.ValidateItemName(itemName);
        if (!itemValidation.IsValid)
        {
            return (false, itemValidation.ErrorMessage, 0);
        }

        var quantityValidation = _validator.ValidateQuantity(quantity);
        if (!quantityValidation.IsValid)
        {
            return (false, quantityValidation.ErrorMessage, 0);
        }

        var priceValidation = _validator.ValidatePrice(startingPrice);
        if (!priceValidation.IsValid)
        {
            return (false, priceValidation.ErrorMessage, 0);
        }

        // Create auction
        var auction = new AuctionItem
        {
            Seller = seller,
            ItemName = itemName,
            Quantity = quantity,
            StartingPrice = startingPrice,
            CurrentBid = startingPrice,
            CurrentBidder = null,
            ExpirationTime = DateTime.UtcNow.AddHours(_config.DefaultAuctionDurationHours),
            IsActive = true
        };

        var auctionId = await _repository.CreateAuctionAsync(auction);

        Console.WriteLine($"[AuctionServer] Auction created: #{auctionId} - {itemName} x{quantity} by {seller} ({startingPrice} gold)");
        return (true, $"Auction created: #{auctionId}", auctionId);
    }

    public async Task<(bool Success, string Message)> PlaceBidAsync(int auctionId, string bidder, int amount)
    {
        // Validate bidder
        var bidderValidation = _validator.ValidateUsername(bidder);
        if (!bidderValidation.IsValid)
        {
            return (false, bidderValidation.ErrorMessage);
        }

        // Get auction
        var auction = await _repository.GetAuctionAsync(auctionId);
        if (auction == null)
        {
            return (false, "Auction not found");
        }

        if (!auction.IsActive)
        {
            return (false, "Auction is not active");
        }

        if (auction.Seller.Equals(bidder, StringComparison.OrdinalIgnoreCase))
        {
            return (false, "Cannot bid on your own auction");
        }

        // Validate bid amount
        var bidValidation = _validator.ValidateBidAmount(amount, auction.CurrentBid);
        if (!bidValidation.IsValid)
        {
            return (false, bidValidation.ErrorMessage);
        }

        // Place bid
        auction.CurrentBid = amount;
        auction.CurrentBidder = bidder;
        await _repository.UpdateAuctionAsync(auction);

        Console.WriteLine($"[AuctionServer] New bid on #{auctionId}: {amount} gold by {bidder}");
        return (true, $"Bid placed: {amount} gold");
    }

    public async Task<(bool Success, string Message)> CancelAuctionAsync(int auctionId, string seller)
    {
        // Validate seller
        var sellerValidation = _validator.ValidateUsername(seller);
        if (!sellerValidation.IsValid)
        {
            return (false, sellerValidation.ErrorMessage);
        }

        // Get auction
        var auction = await _repository.GetAuctionAsync(auctionId);
        if (auction == null)
        {
            return (false, "Auction not found");
        }

        if (!auction.Seller.Equals(seller, StringComparison.OrdinalIgnoreCase))
        {
            return (false, "You can only cancel your own auctions");
        }

        if (auction.CurrentBidder != null)
        {
            return (false, "Cannot cancel auction with active bids");
        }

        // Cancel auction
        auction.IsActive = false;
        await _repository.UpdateAuctionAsync(auction);
        _totalProcessed++;

        Console.WriteLine($"[AuctionServer] Auction cancelled: #{auctionId} by {seller}");
        return (true, "Auction cancelled");
    }

    public Task<IEnumerable<AuctionItem>> GetActiveAuctionsAsync()
    {
        return _repository.GetActiveAuctionsAsync();
    }

    public Task<IEnumerable<AuctionItem>> GetUserAuctionsAsync(string username)
    {
        if (string.IsNullOrWhiteSpace(username))
        {
            return Task.FromResult<IEnumerable<AuctionItem>>(Array.Empty<AuctionItem>());
        }

        return _repository.GetUserAuctionsAsync(username);
    }

    public Task<AuctionItem?> GetAuctionAsync(int auctionId)
    {
        return _repository.GetAuctionAsync(auctionId);
    }

    public async Task CheckExpirationsAsync()
    {
        var allAuctions = await _repository.GetActiveAuctionsAsync();
        var now = DateTime.UtcNow;

        foreach (var auction in allAuctions.Where(a => a.ExpirationTime <= now))
        {
            auction.IsActive = false;
            await _repository.UpdateAuctionAsync(auction);
            _totalProcessed++;

            if (auction.CurrentBidder != null)
            {
                Console.WriteLine($"[AuctionServer] Auction #{auction.AuctionId} completed: " +
                                $"{auction.ItemName} sold to {auction.CurrentBidder} for {auction.CurrentBid} gold");
            }
            else
            {
                Console.WriteLine($"[AuctionServer] Auction #{auction.AuctionId} expired with no bids");
            }
        }
    }
}
