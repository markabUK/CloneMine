using CloneMine.LoginServer.Interfaces;
using CloneMine.LoginServer.Models;

namespace CloneMine.LoginServer.Handlers;

/// <summary>
/// Message handler for processing client requests
/// Single Responsibility: Message routing and processing
/// </summary>
public class LoginMessageHandler : IMessageHandler
{
    private readonly IAuthenticationService _authService;
    private readonly IInputValidator _inputValidator;
    private readonly IEncryptionService _encryptionService;
    private readonly ServerConfiguration _config;

    public LoginMessageHandler(
        IAuthenticationService authService,
        IInputValidator inputValidator,
        IEncryptionService encryptionService,
        ServerConfiguration config)
    {
        _authService = authService ?? throw new ArgumentNullException(nameof(authService));
        _inputValidator = inputValidator ?? throw new ArgumentNullException(nameof(inputValidator));
        _encryptionService = encryptionService ?? throw new ArgumentNullException(nameof(encryptionService));
        _config = config ?? throw new ArgumentNullException(nameof(config));
    }

    public async Task<string> HandleMessageAsync(string message)
    {
        // Validate message
        var validation = _inputValidator.ValidateMessage(message);
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
            "LOGIN" => await HandleLoginAsync(parts),
            "REGISTER" => await HandleRegisterAsync(parts),
            "CHECK_CHARACTERS" => await HandleCheckCharactersAsync(parts),
            "PING" => "PONG",
            _ => "ERROR|Unknown command"
        };
    }

    private async Task<string> HandleLoginAsync(string[] parts)
    {
        if (parts.Length < 3)
        {
            return "ERROR|Invalid LOGIN format. Expected: LOGIN|username|password";
        }

        var username = parts[1];
        var password = parts[2];

        // Null check
        if (username == null || password == null)
        {
            return "ERROR|Username and password cannot be null";
        }

        var result = await _authService.LoginAsync(username, password);

        if (result.Success && result.Account != null)
        {
            Console.WriteLine($"[LoginServer] Login successful: {username} (Characters: {result.Account.CharacterCount})");
            return $"SUCCESS|{username}|{result.Account.CharacterCount}";
        }

        Console.WriteLine($"[LoginServer] Login failed: {username} - {result.Message}");
        return $"ERROR|{result.Message}";
    }

    private async Task<string> HandleRegisterAsync(string[] parts)
    {
        if (parts.Length < 3)
        {
            return "ERROR|Invalid REGISTER format. Expected: REGISTER|username|password";
        }

        var username = parts[1];
        var password = parts[2];

        // Null check
        if (username == null || password == null)
        {
            return "ERROR|Username and password cannot be null";
        }

        var result = await _authService.RegisterAsync(username, password);

        if (result.Success && result.Account != null)
        {
            Console.WriteLine($"[LoginServer] Registration successful: {username}");
            return $"SUCCESS|{username}|{result.Account.CharacterCount}";
        }

        Console.WriteLine($"[LoginServer] Registration failed: {username} - {result.Message}");
        return $"ERROR|{result.Message}";
    }

    private async Task<string> HandleCheckCharactersAsync(string[] parts)
    {
        if (parts.Length < 2)
        {
            return "ERROR|Invalid CHECK_CHARACTERS format. Expected: CHECK_CHARACTERS|username";
        }

        var username = parts[1];

        // Null check
        if (username == null)
        {
            return "ERROR|Username cannot be null";
        }

        var charCount = await _authService.GetCharacterCountAsync(username);
        var available = _config.MaxCharactersPerAccount - charCount;

        Console.WriteLine($"[LoginServer] Character check: {username} has {charCount}/{_config.MaxCharactersPerAccount}");
        return $"SUCCESS|{charCount}|{available}";
    }
}
