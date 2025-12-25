using CloneMine.GameServer.Handlers;
using CloneMine.GameServer.Interfaces;
using CloneMine.GameServer.Models;
using CloneMine.GameServer.Repositories;
using CloneMine.GameServer.Security;
using CloneMine.GameServer.Services;
using CloneMine.GameServer.Validation;
using CloneMine.Common.Interfaces;
using CloneMine.Common.Security;

namespace CloneMine.GameServer;

/// <summary>
/// Entry point for the Game Server
/// Follows Dependency Inversion Principle: Depends on abstractions
/// </summary>
class Program
{
    static async Task Main(string[] args)
    {
        Console.WriteLine("==================================");
        Console.WriteLine("  CloneMine Game Server (.NET 10)");
        Console.WriteLine("  With SOLID Principles & Security");
        Console.WriteLine("==================================");
        Console.WriteLine();

        // Parse command line arguments
        var config = new ServerConfiguration();

        if (args.Length > 0 && int.TryParse(args[0], out int port))
        {
            config.Port = port;
        }

        if (args.Length > 1)
        {
            config.ConfigFile = args[1];
        }

        // Display configuration
        Console.WriteLine("[GameServer] Configuration:");
        Console.WriteLine($"  Port: {config.Port}");
        Console.WriteLine($"  Tick rate: {config.TickRate} Hz");
        Console.WriteLine($"  Tick interval: {config.TickInterval:F4}s");
        Console.WriteLine();
        Console.WriteLine("[GameServer] Security Features:");
        Console.WriteLine("  ✓ AES-256 encryption for network traffic");
        Console.WriteLine("  ✓ Input validation and sanitization");
        Console.WriteLine("  ✓ SQL injection protection");
        Console.WriteLine("  ✓ Null checks on all inputs");
        Console.WriteLine("  ✓ Rate limiting (200 requests per 60 seconds)");
        Console.WriteLine();

        // Dependency Injection - Create all dependencies
        IPlayerRepository playerRepository = new InMemoryPlayerRepository();
        IInputValidator inputValidator = new InputValidator(config);
        IEncryptionService encryptionService = new AesEncryptionService();
        IRateLimiter rateLimiter = new RateLimiter(200, 60); // 200 requests per 60 seconds

        IGameService gameService = new GameService(playerRepository, inputValidator);

        IMessageHandler messageHandler = new GameMessageHandler(gameService, inputValidator);

        IClientHandler clientHandler = new TcpClientHandler(messageHandler, encryptionService, rateLimiter);

        var server = new TcpServerListener(config, clientHandler, gameService);

        // Setup signal handlers
        var cancellationTokenSource = new CancellationTokenSource();
        Console.CancelKeyPress += async (sender, e) =>
        {
            e.Cancel = true;
            Console.WriteLine("\n[GameServer] Shutdown signal received");
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
            Console.WriteLine($"[GameServer] Fatal error: {ex.Message}");
            if (ex.InnerException != null)
            {
                Console.WriteLine($"[GameServer] Inner exception: {ex.InnerException.Message}");
            }
            return;
        }
    }
}
