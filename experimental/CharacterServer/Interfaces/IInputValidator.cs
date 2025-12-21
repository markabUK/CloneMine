namespace CloneMine.CharacterServer.Interfaces;

public interface IInputValidator
{
    (bool IsValid, string ErrorMessage) ValidateCharacterName(string? characterName);
    (bool IsValid, string ErrorMessage) ValidateUsername(string? username);
    (bool IsValid, string ErrorMessage) ValidateMessage(string? message);
}
