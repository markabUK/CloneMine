namespace CloneMine.LoginServer.Interfaces;

/// <summary>
/// Interface for input validation
/// Single Responsibility: Data validation
/// </summary>
public interface IInputValidator
{
    (bool IsValid, string ErrorMessage) ValidateUsername(string? username);
    (bool IsValid, string ErrorMessage) ValidatePassword(string? password);
    (bool IsValid, string ErrorMessage) ValidateMessage(string? message);
}
