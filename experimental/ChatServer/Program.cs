using CloneMine.ChatServer.Handlers;
using CloneMine.ChatServer.Interfaces;
using CloneMine.ChatServer.Models;
using CloneMine.ChatServer.Repositories;
using CloneMine.ChatServer.Security;
using CloneMine.ChatServer.Services;
using CloneMine.ChatServer.Validation;

namespace CloneMine.ChatServer;

/// <summary>
/// Entry point for the Chat Server
/// Follows Dependency Inversion Principle: Depends on abstractions
/// </summary>
class Program
{
    static async Task Main(string[] args)
    {
        Console.WriteLine("==================================");
        Console.WriteLine("  CloneMine Chat Server (.NET 10)");
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
        Console.WriteLine("[ChatServer] Configuration:");
        Console.WriteLine($"  Port: {config.Port}");
        Console.WriteLine($"  Max history: {config.MaxHistory} messages");
        Console.WriteLine($"  Max message length: {config.MaxMessageLength}");
        Console.WriteLine();
        Console.WriteLine("[ChatServer] Security Features:");
        Console.WriteLine("  ✓ AES-256 encryption for network traffic");
        Console.WriteLine("  ✓ Input validation and sanitization");
        Console.WriteLine("  ✓ SQL injection protection");
        Console.WriteLine("  ✓ Null checks on all inputs");
        Console.WriteLine();

        // Dependency Injection - Create all dependencies
        IChatRepository chatRepository = new InMemoryChatRepository(config.MaxHistory);
        IInputValidator inputValidator = new InputValidator(config);
        IEncryptionService encryptionService = new AesEncryptionService();

        IChatService chatService = new ChatService(chatRepository, inputValidator);

        var messageHandler = new ChatMessageHandler(chatService, inputValidator);

        IClientHandler clientHandler = new TcpClientHandler(messageHandler, encryptionService);

        var server = new TcpServerListener(config, clientHandler, messageHandler, encryptionService);

        // Setup signal handlers
        var cancellationTokenSource = new CancellationTokenSource();
        Console.CancelKeyPress += async (sender, e) =>
        {
            e.Cancel = true;
            Console.WriteLine("\n[ChatServer] Shutdown signal received");
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
            Console.WriteLine($"[ChatServer] Fatal error: {ex.Message}");
            if (ex.InnerException != null)
            {
                Console.WriteLine($"[ChatServer] Inner exception: {ex.InnerException.Message}");
            }
            return;
        }
    }
}
