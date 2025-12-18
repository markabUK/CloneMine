using System.Text.RegularExpressions;
using CloneMine.LoginServer.Interfaces;
using CloneMine.LoginServer.Models;

namespace CloneMine.LoginServer.Validation;

/// <summary>
/// Input validator for all user inputs
/// Single Responsibility: Input validation and sanitization
/// </summary>
public class InputValidator : IInputValidator
{
    private readonly ServerConfiguration _config;
    private static readonly Regex UsernameRegex = new Regex(@"^[a-zA-Z0-9_]+$", RegexOptions.Compiled);
    private static readonly Regex SqlInjectionRegex = new Regex(@"('|(;|--)|(\bOR\b)|(\bAND\b)|(\bUNION\b)|(\bSELECT\b)|(\bINSERT\b)|(\bUPDATE\b)|(\bDELETE\b)|(\bDROP\b))", 
        RegexOptions.IgnoreCase | RegexOptions.Compiled);

    public InputValidator(ServerConfiguration config)
    {
        _config = config ?? throw new ArgumentNullException(nameof(config));
    }

    public (bool IsValid, string ErrorMessage) ValidateUsername(string? username)
    {
        // Null check
        if (username == null)
        {
            return (false, "Username cannot be null");
        }

        // Empty check
        if (string.IsNullOrWhiteSpace(username))
        {
            return (false, "Username cannot be empty");
        }

        // Length checks
        if (username.Length < _config.MinUsernameLength)
        {
            return (false, $"Username must be at least {_config.MinUsernameLength} characters");
        }

        if (username.Length > _config.MaxUsernameLength)
        {
            return (false, $"Username must be at most {_config.MaxUsernameLength} characters");
        }

        // Format check - alphanumeric and underscore only
        if (!UsernameRegex.IsMatch(username))
        {
            return (false, "Username can only contain letters, numbers, and underscores");
        }

        // SQL injection check
        if (SqlInjectionRegex.IsMatch(username))
        {
            return (false, "Username contains invalid characters");
        }

        return (true, string.Empty);
    }

    public (bool IsValid, string ErrorMessage) ValidatePassword(string? password)
    {
        // Null check
        if (password == null)
        {
            return (false, "Password cannot be null");
        }

        // Empty check
        if (string.IsNullOrWhiteSpace(password))
        {
            return (false, "Password cannot be empty");
        }

        // Length checks
        if (password.Length < _config.MinPasswordLength)
        {
            return (false, $"Password must be at least {_config.MinPasswordLength} characters");
        }

        if (password.Length > _config.MaxPasswordLength)
        {
            return (false, $"Password must be at most {_config.MaxPasswordLength} characters");
        }

        // Check for null bytes
        if (password.Contains('\0'))
        {
            return (false, "Password contains invalid characters");
        }

        return (true, string.Empty);
    }

    public (bool IsValid, string ErrorMessage) ValidateMessage(string? message)
    {
        // Null check
        if (message == null)
        {
            return (false, "Message cannot be null");
        }

        // Empty check is allowed for some commands
        if (string.IsNullOrWhiteSpace(message))
        {
            return (false, "Message cannot be empty");
        }

        // Check for null bytes
        if (message.Contains('\0'))
        {
            return (false, "Message contains invalid characters");
        }

        // Length check - prevent DoS attacks
        if (message.Length > 4096)
        {
            return (false, "Message is too long");
        }

        // SQL injection check
        if (SqlInjectionRegex.IsMatch(message))
        {
            return (false, "Message contains potentially dangerous content");
        }

        return (true, string.Empty);
    }
}
