using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Collections.Concurrent;

namespace CloneMine.LoginServer;

class Program
{
    private static bool _running = true;
    private static TcpListener? _listener;
    private static readonly ConcurrentDictionary<string, string> _accounts = new();
    private static readonly ConcurrentDictionary<string, int> _characterCounts = new();
    private static int _maxCharactersPerAccount = 5;
    
    static async Task Main(string[] args)
    {
        Console.WriteLine("==================================");
        Console.WriteLine("  CloneMine Login Server (.NET 10)");
        Console.WriteLine("==================================");
        Console.WriteLine();
        
        // Parse command line arguments
        int port = 25564; // Default login port
        string configFile = "server_config.txt";
        
        if (args.Length > 0 && int.TryParse(args[0], out int parsedPort))
        {
            port = parsedPort;
        }
        
        if (args.Length > 1 && int.TryParse(args[1], out int maxChars))
        {
            _maxCharactersPerAccount = maxChars;
        }
        
        if (args.Length > 2)
        {
            configFile = args[2];
        }
        
        // Initialize test accounts
        InitializeTestAccounts();
        
        Console.WriteLine("[LoginServer] Configuration:");
        Console.WriteLine($"  Port: {port}");
        Console.WriteLine($"  Max characters per account: {_maxCharactersPerAccount}");
        Console.WriteLine($"  Config file: {configFile}");
        Console.WriteLine();
        Console.WriteLine("[LoginServer] Test accounts:");
        Console.WriteLine("  Username: test, Password: test123");
        Console.WriteLine("  Username: admin, Password: admin123");
        Console.WriteLine("  Username: player1, Password: password1");
        Console.WriteLine();
        
        // Setup signal handlers
        Console.CancelKeyPress += (sender, e) =>
        {
            e.Cancel = true;
            Console.WriteLine("\n[LoginServer] Shutdown signal received");
            _running = false;
        };
        
        try
        {
            // Start TCP listener
            _listener = new TcpListener(IPAddress.Any, port);
            _listener.Start();
            
            Console.WriteLine($"[LoginServer] Started on port {port}");
            Console.WriteLine("[LoginServer] Press Ctrl+C to stop");
            Console.WriteLine();
            
            // Accept connections asynchronously
            _ = Task.Run(AcceptClientsAsync);
            
            // Main loop
            while (_running)
            {
                await Task.Delay(100);
            }
        }
        catch (Exception ex)
        {
            Console.WriteLine($"[LoginServer] Fatal error: {ex.Message}");
            return;
        }
        finally
        {
            // Shutdown
            Console.WriteLine("[LoginServer] Shutting down...");
            _listener?.Stop();
            Console.WriteLine("[LoginServer] Shutdown complete");
        }
    }
    
    private static void InitializeTestAccounts()
    {
        _accounts["test"] = "test123";
        _accounts["admin"] = "admin123";
        _accounts["player1"] = "password1";
        
        _characterCounts["test"] = 2;
        _characterCounts["admin"] = 1;
        _characterCounts["player1"] = 0;
    }
    
    private static async Task AcceptClientsAsync()
    {
        while (_running && _listener != null)
        {
            try
            {
                var client = await _listener.AcceptTcpClientAsync();
                Console.WriteLine($"[LoginServer] Client connected: {client.Client.RemoteEndPoint}");
                
                // Handle client in separate task
                _ = Task.Run(() => HandleClientAsync(client));
            }
            catch (Exception ex) when (_running)
            {
                Console.WriteLine($"[LoginServer] Error accepting client: {ex.Message}");
            }
        }
    }
    
    private static async Task HandleClientAsync(TcpClient client)
    {
        try
        {
            using var stream = client.GetStream();
            var buffer = new byte[1024];
            
            while (_running && client.Connected)
            {
                var bytesRead = await stream.ReadAsync(buffer, 0, buffer.Length);
                if (bytesRead == 0) break;
                
                var message = Encoding.UTF8.GetString(buffer, 0, bytesRead).Trim();
                var response = ProcessMessage(message);
                
                var responseBytes = Encoding.UTF8.GetBytes(response + "\n");
                await stream.WriteAsync(responseBytes, 0, responseBytes.Length);
            }
        }
        catch (Exception ex)
        {
            Console.WriteLine($"[LoginServer] Client error: {ex.Message}");
        }
        finally
        {
            client.Close();
            Console.WriteLine($"[LoginServer] Client disconnected");
        }
    }
    
    private static string ProcessMessage(string message)
    {
        var parts = message.Split('|');
        if (parts.Length < 1) return "ERROR|Invalid message format";
        
        var command = parts[0].ToUpper();
        
        return command switch
        {
            "LOGIN" when parts.Length >= 3 => ProcessLogin(parts[1], parts[2]),
            "REGISTER" when parts.Length >= 3 => ProcessRegister(parts[1], parts[2]),
            "CHECK_CHARACTERS" when parts.Length >= 2 => ProcessCheckCharacters(parts[1]),
            _ => "ERROR|Unknown command"
        };
    }
    
    private static string ProcessLogin(string username, string password)
    {
        if (_accounts.TryGetValue(username, out var storedPassword))
        {
            if (storedPassword == password)
            {
                var charCount = _characterCounts.GetValueOrDefault(username, 0);
                Console.WriteLine($"[LoginServer] Login successful: {username} (Characters: {charCount})");
                return $"SUCCESS|{username}|{charCount}";
            }
        }
        
        Console.WriteLine($"[LoginServer] Login failed: {username}");
        return "ERROR|Invalid credentials";
    }
    
    private static string ProcessRegister(string username, string password)
    {
        if (_accounts.ContainsKey(username))
        {
            Console.WriteLine($"[LoginServer] Registration failed: {username} already exists");
            return "ERROR|Username already exists";
        }
        
        if (string.IsNullOrWhiteSpace(username) || username.Length < 3)
        {
            return "ERROR|Username must be at least 3 characters";
        }
        
        if (string.IsNullOrWhiteSpace(password) || password.Length < 6)
        {
            return "ERROR|Password must be at least 6 characters";
        }
        
        _accounts[username] = password;
        _characterCounts[username] = 0;
        
        Console.WriteLine($"[LoginServer] Registration successful: {username}");
        return $"SUCCESS|{username}|0";
    }
    
    private static string ProcessCheckCharacters(string username)
    {
        var charCount = _characterCounts.GetValueOrDefault(username, 0);
        var available = _maxCharactersPerAccount - charCount;
        
        Console.WriteLine($"[LoginServer] Character check: {username} has {charCount}/{_maxCharactersPerAccount}");
        return $"SUCCESS|{charCount}|{available}";
    }
}
