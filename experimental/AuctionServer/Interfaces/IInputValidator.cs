namespace CloneMine.AuctionServer.Interfaces;

/// <summary>
/// Interface for input validation
/// Single Responsibility: Data validation
/// </summary>
public interface IInputValidator
{
    (bool IsValid, string ErrorMessage) ValidateItemName(string? itemName);
    (bool IsValid, string ErrorMessage) ValidateUsername(string? username);
    (bool IsValid, string ErrorMessage) ValidateQuantity(int quantity);
    (bool IsValid, string ErrorMessage) ValidatePrice(int price);
    (bool IsValid, string ErrorMessage) ValidateBidAmount(int amount, int currentBid);
    (bool IsValid, string ErrorMessage) ValidateMessage(string? message);
}
