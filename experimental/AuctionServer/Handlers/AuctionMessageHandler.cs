using CloneMine.AuctionServer.Interfaces;
using CloneMine.AuctionServer.Models;

namespace CloneMine.AuctionServer.Handlers;

/// <summary>
/// Message handler for processing auction requests
/// Single Responsibility: Message routing and processing
/// </summary>
public class AuctionMessageHandler : IMessageHandler
{
    private readonly IAuctionService _auctionService;
    private readonly IInputValidator _validator;

    public AuctionMessageHandler(
        IAuctionService auctionService,
        IInputValidator validator)
    {
        _auctionService = auctionService ?? throw new ArgumentNullException(nameof(auctionService));
        _validator = validator ?? throw new ArgumentNullException(nameof(validator));
    }

    public async Task<string> HandleMessageAsync(string message)
    {
        // Validate message
        var validation = _validator.ValidateMessage(message);
        if (!validation.IsValid)
        {
            return $"ERROR|{validation.ErrorMessage}";
        }

        // Parse message
        var parts = message.Split('|', StringSplitOptions.None);
        if (parts.Length < 1)
        {
            return "ERROR|Invalid message format";
        }

        var command = parts[0]?.ToUpperInvariant();
        if (string.IsNullOrWhiteSpace(command))
        {
            return "ERROR|Command cannot be empty";
        }

        // Route to appropriate handler
        return command switch
        {
            "LIST_AUCTIONS" => await HandleListAuctionsAsync(),
            "CREATE_AUCTION" => await HandleCreateAuctionAsync(parts),
            "PLACE_BID" => await HandlePlaceBidAsync(parts),
            "GET_AUCTION" => await HandleGetAuctionAsync(parts),
            "CANCEL_AUCTION" => await HandleCancelAuctionAsync(parts),
            "GET_MY_AUCTIONS" => await HandleGetMyAuctionsAsync(parts),
            "PING" => "PONG",
            _ => "ERROR|Unknown command"
        };
    }

    private async Task<string> HandleListAuctionsAsync()
    {
        var auctions = await _auctionService.GetActiveAuctionsAsync();
        var auctionList = auctions.ToList();

        if (auctionList.Count == 0)
        {
            return "AUCTIONS|NONE";
        }

        var list = string.Join(";", auctionList.Select(a =>
            $"{a.AuctionId}:{a.ItemName}:{a.Quantity}:{a.CurrentBid}:{a.Seller}"));

        return $"AUCTIONS|{list}";
    }

    private async Task<string> HandleCreateAuctionAsync(string[] parts)
    {
        if (parts.Length < 5)
        {
            return "ERROR|Invalid CREATE_AUCTION format. Expected: CREATE_AUCTION|seller|itemName|quantity|price";
        }

        var seller = parts[1];
        var itemName = parts[2];

        if (!int.TryParse(parts[3], out int quantity))
        {
            return "ERROR|Invalid quantity";
        }

        if (!int.TryParse(parts[4], out int price))
        {
            return "ERROR|Invalid price";
        }

        var result = await _auctionService.CreateAuctionAsync(seller, itemName, quantity, price);
        return result.Success 
            ? $"SUCCESS|{result.Message}"
            : $"ERROR|{result.Message}";
    }

    private async Task<string> HandlePlaceBidAsync(string[] parts)
    {
        if (parts.Length < 3)
        {
            return "ERROR|Invalid PLACE_BID format. Expected: PLACE_BID|auctionId|bidder:amount";
        }

        if (!int.TryParse(parts[1], out int auctionId))
        {
            return "ERROR|Invalid auction ID";
        }

        var bidParts = parts[2].Split(':');
        if (bidParts.Length != 2)
        {
            return "ERROR|Invalid bid data";
        }

        var bidder = bidParts[0];
        if (!int.TryParse(bidParts[1], out int amount))
        {
            return "ERROR|Invalid bid amount";
        }

        var result = await _auctionService.PlaceBidAsync(auctionId, bidder, amount);
        return result.Success
            ? $"SUCCESS|{result.Message}"
            : $"ERROR|{result.Message}";
    }

    private async Task<string> HandleGetAuctionAsync(string[] parts)
    {
        if (parts.Length < 2)
        {
            return "ERROR|Invalid GET_AUCTION format. Expected: GET_AUCTION|auctionId";
        }

        if (!int.TryParse(parts[1], out int auctionId))
        {
            return "ERROR|Invalid auction ID";
        }

        var auction = await _auctionService.GetAuctionAsync(auctionId);
        if (auction == null)
        {
            return "ERROR|Auction not found";
        }

        var timeRemaining = auction.ExpirationTime - DateTime.UtcNow;
        var hoursRemaining = (int)Math.Max(0, timeRemaining.TotalHours);

        var auctionData = $"{auction.AuctionId}|{auction.ItemName}|{auction.Quantity}|" +
                         $"{auction.StartingPrice}|{auction.CurrentBid}|{auction.CurrentBidder ?? "None"}|" +
                         $"{auction.Seller}|{hoursRemaining}|{auction.IsActive}";

        return $"AUCTION|{auctionData}";
    }

    private async Task<string> HandleCancelAuctionAsync(string[] parts)
    {
        if (parts.Length < 3)
        {
            return "ERROR|Invalid CANCEL_AUCTION format. Expected: CANCEL_AUCTION|seller|auctionId";
        }

        var seller = parts[1];
        if (!int.TryParse(parts[2], out int auctionId))
        {
            return "ERROR|Invalid auction ID";
        }

        var result = await _auctionService.CancelAuctionAsync(auctionId, seller);
        return result.Success
            ? $"SUCCESS|{result.Message}"
            : $"ERROR|{result.Message}";
    }

    private async Task<string> HandleGetMyAuctionsAsync(string[] parts)
    {
        if (parts.Length < 2)
        {
            return "ERROR|Invalid GET_MY_AUCTIONS format. Expected: GET_MY_AUCTIONS|username";
        }

        var username = parts[1];
        var auctions = await _auctionService.GetUserAuctionsAsync(username);
        var auctionList = auctions.ToList();

        if (auctionList.Count == 0)
        {
            return "MY_AUCTIONS|NONE";
        }

        var list = string.Join(";", auctionList.Select(a =>
            $"{a.AuctionId}:{a.ItemName}:{a.Quantity}:{a.CurrentBid}"));

        return $"MY_AUCTIONS|{list}";
    }
}
