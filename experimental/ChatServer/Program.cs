using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Collections.Concurrent;

namespace CloneMine.ChatServer;

class Program
{
    private static bool _running = true;
    private static TcpListener? _listener;
    private static readonly ConcurrentBag<TcpClient> _clients = new();
    private static readonly List<string> _messageHistory = new();
    private static readonly int _maxHistory = 100;
    private static readonly object _historyLock = new();
    
    static async Task Main(string[] args)
    {
        Console.WriteLine("==================================");
        Console.WriteLine("  CloneMine Chat Server (.NET 10)");
        Console.WriteLine("==================================");
        Console.WriteLine();
        
        // Parse command line arguments
        int port = 25566; // Default chat port
        string configFile = "server_config.txt";
        
        if (args.Length > 0 && int.TryParse(args[0], out int parsedPort))
        {
            port = parsedPort;
        }
        
        if (args.Length > 1)
        {
            configFile = args[1];
        }
        
        Console.WriteLine("[ChatServer] Configuration:");
        Console.WriteLine($"  Port: {port}");
        Console.WriteLine($"  Max history: {_maxHistory} messages");
        Console.WriteLine($"  Config file: {configFile}");
        Console.WriteLine();
        
        // Setup signal handlers
        Console.CancelKeyPress += (sender, e) =>
        {
            e.Cancel = true;
            Console.WriteLine("\n[ChatServer] Shutdown signal received");
            _running = false;
        };
        
        try
        {
            // Start TCP listener
            _listener = new TcpListener(IPAddress.Any, port);
            _listener.Start();
            
            Console.WriteLine($"[ChatServer] Started on port {port}");
            Console.WriteLine("[ChatServer] Press Ctrl+C to stop");
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
            Console.WriteLine($"[ChatServer] Fatal error: {ex.Message}");
            return;
        }
        finally
        {
            // Shutdown
            Console.WriteLine("[ChatServer] Shutting down...");
            _listener?.Stop();
            
            // Close all client connections
            foreach (var client in _clients)
            {
                client.Close();
            }
            
            Console.WriteLine("[ChatServer] Shutdown complete");
        }
    }
    
    private static async Task AcceptClientsAsync()
    {
        while (_running && _listener != null)
        {
            try
            {
                var client = await _listener.AcceptTcpClientAsync();
                Console.WriteLine($"[ChatServer] Client connected: {client.Client.RemoteEndPoint}");
                
                _clients.Add(client);
                
                // Send chat history to new client
                _ = Task.Run(() => SendHistoryToClient(client));
                
                // Handle client in separate task
                _ = Task.Run(() => HandleClientAsync(client));
            }
            catch (Exception ex) when (_running)
            {
                Console.WriteLine($"[ChatServer] Error accepting client: {ex.Message}");
            }
        }
    }
    
    private static async Task SendHistoryToClient(TcpClient client)
    {
        try
        {
            lock (_historyLock)
            {
                if (_messageHistory.Count > 0)
                {
                    var stream = client.GetStream();
                    var historyMessage = "HISTORY|" + string.Join("|", _messageHistory) + "\n";
                    var bytes = Encoding.UTF8.GetBytes(historyMessage);
                    stream.Write(bytes, 0, bytes.Length);
                }
            }
        }
        catch (Exception ex)
        {
            Console.WriteLine($"[ChatServer] Error sending history: {ex.Message}");
        }
    }
    
    private static async Task HandleClientAsync(TcpClient client)
    {
        string? username = null;
        
        try
        {
            using var stream = client.GetStream();
            var buffer = new byte[1024];
            
            while (_running && client.Connected)
            {
                var bytesRead = await stream.ReadAsync(buffer, 0, buffer.Length);
                if (bytesRead == 0) break;
                
                var message = Encoding.UTF8.GetString(buffer, 0, bytesRead).Trim();
                
                // Parse message format: COMMAND|username|message
                var parts = message.Split('|', 3);
                if (parts.Length >= 2)
                {
                    var command = parts[0].ToUpper();
                    
                    if (command == "JOIN" && parts.Length >= 2)
                    {
                        username = parts[1];
                        Console.WriteLine($"[ChatServer] {username} joined");
                        BroadcastMessage($"SYSTEM|{username} joined the chat");
                    }
                    else if (command == "CHAT" && parts.Length >= 3)
                    {
                        username = parts[1];
                        var chatMessage = parts[2];
                        
                        if (chatMessage.Length > 256)
                        {
                            chatMessage = chatMessage.Substring(0, 256);
                        }
                        
                        Console.WriteLine($"[ChatServer] {username}: {chatMessage}");
                        BroadcastMessage($"CHAT|{username}|{chatMessage}");
                        
                        // Add to history
                        lock (_historyLock)
                        {
                            _messageHistory.Add($"{username}: {chatMessage}");
                            if (_messageHistory.Count > _maxHistory)
                            {
                                _messageHistory.RemoveAt(0);
                            }
                        }
                    }
                }
            }
        }
        catch (Exception ex)
        {
            Console.WriteLine($"[ChatServer] Client error: {ex.Message}");
        }
        finally
        {
            if (username != null)
            {
                Console.WriteLine($"[ChatServer] {username} disconnected");
                BroadcastMessage($"SYSTEM|{username} left the chat");
            }
            
            client.Close();
        }
    }
    
    private static void BroadcastMessage(string message)
    {
        var messageBytes = Encoding.UTF8.GetBytes(message + "\n");
        
        foreach (var client in _clients)
        {
            try
            {
                if (client.Connected)
                {
                    var stream = client.GetStream();
                    stream.Write(messageBytes, 0, messageBytes.Length);
                }
            }
            catch (Exception)
            {
                // Client disconnected, will be handled by its handler
            }
        }
    }
}
