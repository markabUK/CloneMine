using CloneMine.CharacterServer.Handlers;
using CloneMine.CharacterServer.Interfaces;
using CloneMine.CharacterServer.Models;
using CloneMine.CharacterServer.Repositories;
using CloneMine.CharacterServer.Security;
using CloneMine.CharacterServer.Services;
using CloneMine.CharacterServer.Validation;

namespace CloneMine.CharacterServer;

/// <summary>
/// Entry point for the Character Server
/// Follows Dependency Inversion Principle: Depends on abstractions
/// </summary>
class Program
{
    static async Task Main(string[] args)
    {
        Console.WriteLine("==================================");
        Console.WriteLine("  CloneMine Character Server (.NET 10)");
        Console.WriteLine("  With SOLID Principles & Security");
        Console.WriteLine("==================================");
        Console.WriteLine();

        // Parse command line arguments
        var config = new ServerConfiguration();

        if (args.Length > 0 && int.TryParse(args[0], out int port))
        {
            config.Port = port;
        }

        if (args.Length > 1 && int.TryParse(args[1], out int maxChars))
        {
            config.MaxCharactersPerAccount = maxChars;
        }

        if (args.Length > 2)
        {
            config.ConfigFile = args[2];
        }

        // Display configuration
        Console.WriteLine("[CharacterServer] Configuration:");
        Console.WriteLine($"  Port: {config.Port}");
        Console.WriteLine($"  Max characters per account: {config.MaxCharactersPerAccount}");
        Console.WriteLine($"  Character name length: {config.MinCharacterNameLength}-{config.MaxCharacterNameLength}");
        Console.WriteLine();
        Console.WriteLine("[CharacterServer] Security Features:");
        Console.WriteLine("  ✓ AES-256 encryption for network traffic");
        Console.WriteLine("  ✓ Input validation and sanitization");
        Console.WriteLine("  ✓ SQL injection protection");
        Console.WriteLine("  ✓ Null checks on all inputs");
        Console.WriteLine();

        // Dependency Injection - Create all dependencies
        ICharacterRepository characterRepository = new InMemoryCharacterRepository();
        IInputValidator inputValidator = new InputValidator(config);
        IEncryptionService encryptionService = new AesEncryptionService();

        ICharacterService characterService = new CharacterService(
            characterRepository,
            inputValidator,
            config);

        IMessageHandler messageHandler = new CharacterMessageHandler(
            characterService,
            inputValidator);

        IClientHandler clientHandler = new TcpClientHandler(
            messageHandler,
            encryptionService);

        var server = new TcpServerListener(config, clientHandler);

        // Setup signal handlers
        var cancellationTokenSource = new CancellationTokenSource();
        Console.CancelKeyPress += async (sender, e) =>
        {
            e.Cancel = true;
            Console.WriteLine("\n[CharacterServer] Shutdown signal received");
            await server.StopAsync();
            cancellationTokenSource.Cancel();
        };

        try
        {
            // Start server
            await server.StartAsync(config.Port);
        }
        catch (Exception ex)
        {
            Console.WriteLine($"[CharacterServer] Fatal error: {ex.Message}");
            if (ex.InnerException != null)
            {
                Console.WriteLine($"[CharacterServer] Inner exception: {ex.InnerException.Message}");
            }
            return;
        }
    }
}
