using System.Text.RegularExpressions;
using CloneMine.ChatServer.Interfaces;
using CloneMine.ChatServer.Models;

namespace CloneMine.ChatServer.Validation;

public class InputValidator : IInputValidator
{
    private readonly ServerConfiguration _config;
    private static readonly Regex SqlInjectionRegex = new Regex(@"('|(;|--)|(\bOR\b)|(\bAND\b)|(\bUNION\b)|(\bSELECT\b)|(\bINSERT\b)|(\bUPDATE\b)|(\bDELETE\b)|(\bDROP\b))", 
        RegexOptions.IgnoreCase | RegexOptions.Compiled);

    public InputValidator(ServerConfiguration config)
    {
        _config = config ?? throw new ArgumentNullException(nameof(config));
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
        if (message.Contains('\0')) return (false, "Message contains null bytes");
        if (message.Length > _config.MaxMessageLength) return (false, $"Message must be at most {_config.MaxMessageLength} characters");
        return (true, string.Empty);
    }
}
