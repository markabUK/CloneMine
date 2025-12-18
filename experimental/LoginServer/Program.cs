using CloneMine.LoginServer.Handlers;
using CloneMine.LoginServer.Interfaces;
using CloneMine.LoginServer.Models;
using CloneMine.LoginServer.Repositories;
using CloneMine.LoginServer.Security;
using CloneMine.LoginServer.Services;
using CloneMine.LoginServer.Validation;

namespace CloneMine.LoginServer;

/// <summary>
/// Entry point for the Login Server
/// Follows Dependency Inversion Principle: Depends on abstractions
/// </summary>
class Program
{
    static async Task Main(string[] args)
    {
        Console.WriteLine("==================================");
        Console.WriteLine("  CloneMine Login Server (.NET 10)");
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
        Console.WriteLine("[LoginServer] Configuration:");
        Console.WriteLine($"  Port: {config.Port}");
        Console.WriteLine($"  Max characters per account: {config.MaxCharactersPerAccount}");
        Console.WriteLine($"  Config file: {config.ConfigFile}");
        Console.WriteLine($"  Username length: {config.MinUsernameLength}-{config.MaxUsernameLength}");
        Console.WriteLine($"  Password length: {config.MinPasswordLength}-{config.MaxPasswordLength}");
        Console.WriteLine();
        Console.WriteLine("[LoginServer] Security Features:");
        Console.WriteLine("  ✓ AES-256 encryption for network traffic");
        Console.WriteLine("  ✓ PBKDF2-SHA256 password hashing");
        Console.WriteLine("  ✓ Input validation and sanitization");
        Console.WriteLine("  ✓ SQL injection protection");
        Console.WriteLine("  ✓ Null checks on all inputs");
        Console.WriteLine();
        Console.WriteLine("[LoginServer] Test accounts:");
        Console.WriteLine("  Username: test, Password: test123");
        Console.WriteLine("  Username: admin, Password: admin123");
        Console.WriteLine("  Username: player1, Password: password1");
        Console.WriteLine();

        // Dependency Injection - Create all dependencies
        IAccountRepository accountRepository = new InMemoryAccountRepository();
        IPasswordHasher passwordHasher = new PasswordHasher();
        IInputValidator inputValidator = new InputValidator(config);
        IEncryptionService encryptionService = new AesEncryptionService();
        
        IAuthenticationService authService = new AuthenticationService(
            accountRepository,
            passwordHasher,
            inputValidator);
        
        IMessageHandler messageHandler = new LoginMessageHandler(
            authService,
            inputValidator,
            encryptionService,
            config);
        
        IClientHandler clientHandler = new TcpClientHandler(
            messageHandler,
            encryptionService);
        
        var server = new TcpServerListener(config, clientHandler);

        // Setup signal handlers
        var cancellationTokenSource = new CancellationTokenSource();
        Console.CancelKeyPress += async (sender, e) =>
        {
            e.Cancel = true;
            Console.WriteLine("\n[LoginServer] Shutdown signal received");
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
            Console.WriteLine($"[LoginServer] Fatal error: {ex.Message}");
            if (ex.InnerException != null)
            {
                Console.WriteLine($"[LoginServer] Inner exception: {ex.InnerException.Message}");
            }
            return;
        }
    }
}
