using CloneMine.AuctionServer.Handlers;
using CloneMine.AuctionServer.Interfaces;
using CloneMine.AuctionServer.Models;
using CloneMine.AuctionServer.Repositories;
using CloneMine.AuctionServer.Security;
using CloneMine.AuctionServer.Services;
using CloneMine.AuctionServer.Validation;

namespace CloneMine.AuctionServer;

/// <summary>
/// Entry point for the Auction Server
/// Follows Dependency Inversion Principle: Depends on abstractions
/// </summary>
class Program
{
    static async Task Main(string[] args)
    {
        Console.WriteLine("==================================");
        Console.WriteLine("  CloneMine Auction Server (.NET 10)");
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
        Console.WriteLine("[AuctionServer] Configuration:");
        Console.WriteLine($"  Port: {config.Port}");
        Console.WriteLine($"  Auction duration: {config.DefaultAuctionDurationHours} hours");
        Console.WriteLine($"  Max item name length: {config.MaxItemNameLength}");
        Console.WriteLine($"  Min bid increment: {config.MinBidIncrement} gold");
        Console.WriteLine();
        Console.WriteLine("[AuctionServer] Security Features:");
        Console.WriteLine("  ✓ AES-256 encryption for network traffic");
        Console.WriteLine("  ✓ Input validation and sanitization");
        Console.WriteLine("  ✓ SQL injection protection");
        Console.WriteLine("  ✓ Null checks on all inputs");
        Console.WriteLine("  ✓ Rate limiting (100 requests per 60 seconds)");
        Console.WriteLine();

        // Dependency Injection - Create all dependencies
        IAuctionRepository auctionRepository = new InMemoryAuctionRepository();
        IInputValidator inputValidator = new InputValidator(config);
        CloneMine.AuctionServer.Interfaces.IEncryptionService encryptionService = new AesEncryptionService();
        CloneMine.Common.Interfaces.IRateLimiter rateLimiter = new CloneMine.Common.Security.RateLimiter(100, 60); // 100 requests per 60 seconds

        IAuctionService auctionService = new AuctionService(
            auctionRepository,
            inputValidator,
            config);

        IMessageHandler messageHandler = new AuctionMessageHandler(
            auctionService,
            inputValidator);

        IClientHandler clientHandler = new TcpClientHandler(
            messageHandler,
            encryptionService,
            rateLimiter);

        var server = new TcpServerListener(config, clientHandler, auctionService);

        // Setup signal handlers
        var cancellationTokenSource = new CancellationTokenSource();
        Console.CancelKeyPress += async (sender, e) =>
        {
            e.Cancel = true;
            Console.WriteLine("\n[AuctionServer] Shutdown signal received");
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
            Console.WriteLine($"[AuctionServer] Fatal error: {ex.Message}");
            if (ex.InnerException != null)
            {
                Console.WriteLine($"[AuctionServer] Inner exception: {ex.InnerException.Message}");
            }
            return;
        }
    }
}
