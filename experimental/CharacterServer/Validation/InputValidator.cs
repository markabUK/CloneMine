using System.Text.RegularExpressions;
using CloneMine.CharacterServer.Interfaces;
using CloneMine.CharacterServer.Models;

namespace CloneMine.CharacterServer.Validation;

public class InputValidator : IInputValidator
{
    private readonly ServerConfiguration _config;
    private static readonly Regex AlphanumericRegex = new Regex(@"^[a-zA-Z0-9_]+$", RegexOptions.Compiled);
    private static readonly Regex SqlInjectionRegex = new Regex(@"('|(;|--)|(\bOR\b)|(\bAND\b)|(\bUNION\b)|(\bSELECT\b)|(\bINSERT\b)|(\bUPDATE\b)|(\bDELETE\b)|(\bDROP\b))", 
        RegexOptions.IgnoreCase | RegexOptions.Compiled);

    public InputValidator(ServerConfiguration config)
    {
        _config = config ?? throw new ArgumentNullException(nameof(config));
    }

    public (bool IsValid, string ErrorMessage) ValidateCharacterName(string? characterName)
    {
        if (characterName == null) return (false, "Character name cannot be null");
        if (string.IsNullOrWhiteSpace(characterName)) return (false, "Character name cannot be empty");
        if (characterName.Length < _config.MinCharacterNameLength) 
            return (false, $"Character name must be at least {_config.MinCharacterNameLength} characters");
        if (characterName.Length > _config.MaxCharacterNameLength) 
            return (false, $"Character name must be at most {_config.MaxCharacterNameLength} characters");
        if (!AlphanumericRegex.IsMatch(characterName)) 
            return (false, "Character name can only contain letters, numbers, and underscores");
        if (SqlInjectionRegex.IsMatch(characterName)) return (false, "Character name contains invalid characters");
        if (characterName.Contains('\0')) return (false, "Character name contains null bytes");
        return (true, string.Empty);
    }

    public (bool IsValid, string ErrorMessage) ValidateUsername(string? username)
    {
        if (username == null) return (false, "Username cannot be null");
        if (string.IsNullOrWhiteSpace(username)) return (false, "Username cannot be empty");
        if (username.Length < 3 || username.Length > 16) return (false, "Username must be between 3 and 16 characters");
        if (SqlInjectionRegex.IsMatch(username)) return (false, "Username contains invalid characters");
        if (username.Contains('\0')) return (false, "Username contains null bytes");
        return (true, string.Empty);
    }

    public (bool IsValid, string ErrorMessage) ValidateMessage(string? message)
    {
        if (message == null) return (false, "Message cannot be null");
        if (string.IsNullOrWhiteSpace(message)) return (false, "Message cannot be empty");
        if (message.Contains('\0')) return (false, "Message contains invalid characters");
        if (message.Length > 4096) return (false, "Message is too long");
        if (SqlInjectionRegex.IsMatch(message)) return (false, "Message contains potentially dangerous content");
        return (true, string.Empty);
    }
}
