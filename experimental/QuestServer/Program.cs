using CloneMine.QuestServer.Handlers;
using CloneMine.QuestServer.Interfaces;
using CloneMine.QuestServer.Models;
using CloneMine.QuestServer.Repositories;
using CloneMine.QuestServer.Security;
using CloneMine.QuestServer.Services;
using CloneMine.QuestServer.Validation;
using CloneMine.Common.Interfaces;
using CloneMine.Common.Security;

namespace CloneMine.QuestServer;

/// <summary>
/// Entry point for the Quest Server
/// Follows Dependency Inversion Principle: Depends on abstractions
/// </summary>
class Program
{
    static async Task Main(string[] args)
    {
        Console.WriteLine("==================================");
        Console.WriteLine("  CloneMine Quest Server (.NET 10)");
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
        Console.WriteLine("[QuestServer] Configuration:");
        Console.WriteLine($"  Port: {config.Port}");
        Console.WriteLine();
        Console.WriteLine("[QuestServer] Security Features:");
        Console.WriteLine("  ✓ AES-256 encryption for network traffic");
        Console.WriteLine("  ✓ Input validation and sanitization");
        Console.WriteLine("  ✓ SQL injection protection");
        Console.WriteLine("  ✓ Null checks on all inputs");
        Console.WriteLine("  ✓ Rate limiting (100 requests per 60 seconds)");
        Console.WriteLine();

        // Dependency Injection - Create all dependencies
        IQuestRepository questRepository = new InMemoryQuestRepository();
        IProgressRepository progressRepository = new InMemoryProgressRepository();
        IInputValidator inputValidator = new InputValidator(config);
        IEncryptionService encryptionService = new AesEncryptionService();
        IRateLimiter rateLimiter = new RateLimiter(100, 60); // 100 requests per 60 seconds

        IQuestService questService = new QuestService(
            questRepository,
            progressRepository,
            inputValidator);

        IMessageHandler messageHandler = new QuestMessageHandler(questService, inputValidator);

        IClientHandler clientHandler = new TcpClientHandler(messageHandler, encryptionService, rateLimiter);

        var server = new TcpServerListener(config, clientHandler);

        // Setup signal handlers
        var cancellationTokenSource = new CancellationTokenSource();
        Console.CancelKeyPress += async (sender, e) =>
        {
            e.Cancel = true;
            Console.WriteLine("\n[QuestServer] Shutdown signal received");
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
            Console.WriteLine($"[QuestServer] Fatal error: {ex.Message}");
            if (ex.InnerException != null)
            {
                Console.WriteLine($"[QuestServer] Inner exception: {ex.InnerException.Message}");
            }
            return;
        }
    }
}
