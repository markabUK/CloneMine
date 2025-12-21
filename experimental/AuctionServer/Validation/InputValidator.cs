using System.Text.RegularExpressions;
using CloneMine.AuctionServer.Interfaces;
using CloneMine.AuctionServer.Models;

namespace CloneMine.AuctionServer.Validation;

/// <summary>
/// Input validator for auction inputs
/// Single Responsibility: Input validation and sanitization
/// </summary>
public class InputValidator : IInputValidator
{
    private readonly ServerConfiguration _config;
    private static readonly Regex AlphanumericRegex = new Regex(@"^[a-zA-Z0-9_ ]+$", RegexOptions.Compiled);
    private static readonly Regex SqlInjectionRegex = new Regex(@"('|(;|--)|(\bOR\b)|(\bAND\b)|(\bUNION\b)|(\bSELECT\b)|(\bINSERT\b)|(\bUPDATE\b)|(\bDELETE\b)|(\bDROP\b))", 
        RegexOptions.IgnoreCase | RegexOptions.Compiled);

    public InputValidator(ServerConfiguration config)
    {
        _config = config ?? throw new ArgumentNullException(nameof(config));
    }

    public (bool IsValid, string ErrorMessage) ValidateItemName(string? itemName)
    {
        if (itemName == null)
        {
            return (false, "Item name cannot be null");
        }

        if (string.IsNullOrWhiteSpace(itemName))
        {
            return (false, "Item name cannot be empty");
        }

        if (itemName.Length > _config.MaxItemNameLength)
        {
            return (false, $"Item name must be at most {_config.MaxItemNameLength} characters");
        }

        if (!AlphanumericRegex.IsMatch(itemName))
        {
            return (false, "Item name can only contain letters, numbers, spaces, and underscores");
        }

        if (SqlInjectionRegex.IsMatch(itemName))
        {
            return (false, "Item name contains invalid characters");
        }

        if (itemName.Contains('\0'))
        {
            return (false, "Item name contains null bytes");
        }

        return (true, string.Empty);
    }

    public (bool IsValid, string ErrorMessage) ValidateUsername(string? username)
    {
        if (username == null)
        {
            return (false, "Username cannot be null");
        }

        if (string.IsNullOrWhiteSpace(username))
        {
            return (false, "Username cannot be empty");
        }

        if (username.Length < 3 || username.Length > 16)
        {
            return (false, "Username must be between 3 and 16 characters");
        }

        if (SqlInjectionRegex.IsMatch(username))
        {
            return (false, "Username contains invalid characters");
        }

        if (username.Contains('\0'))
        {
            return (false, "Username contains null bytes");
        }

        return (true, string.Empty);
    }

    public (bool IsValid, string ErrorMessage) ValidateQuantity(int quantity)
    {
        if (quantity <= 0)
        {
            return (false, "Quantity must be greater than 0");
        }

        if (quantity > 1000)
        {
            return (false, "Quantity cannot exceed 1000");
        }

        return (true, string.Empty);
    }

    public (bool IsValid, string ErrorMessage) ValidatePrice(int price)
    {
        if (price <= 0)
        {
            return (false, "Price must be greater than 0");
        }

        if (price > 1000000)
        {
            return (false, "Price cannot exceed 1,000,000");
        }

        return (true, string.Empty);
    }

    public (bool IsValid, string ErrorMessage) ValidateBidAmount(int amount, int currentBid)
    {
        if (amount <= 0)
        {
            return (false, "Bid amount must be greater than 0");
        }

        if (amount <= currentBid)
        {
            return (false, $"Bid must be higher than current bid ({currentBid} gold)");
        }

        if (amount < currentBid + _config.MinBidIncrement)
        {
            return (false, $"Bid must be at least {_config.MinBidIncrement} gold higher than current bid");
        }

        return (true, string.Empty);
    }

    public (bool IsValid, string ErrorMessage) ValidateMessage(string? message)
    {
        if (message == null)
        {
            return (false, "Message cannot be null");
        }

        if (string.IsNullOrWhiteSpace(message))
        {
            return (false, "Message cannot be empty");
        }

        if (message.Contains('\0'))
        {
            return (false, "Message contains invalid characters");
        }

        if (message.Length > 4096)
        {
            return (false, "Message is too long");
        }

        if (SqlInjectionRegex.IsMatch(message))
        {
            return (false, "Message contains potentially dangerous content");
        }

        return (true, string.Empty);
    }
}
