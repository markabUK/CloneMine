using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Collections.Concurrent;
using System.Numerics;

namespace CloneMine.GameServer;

class Player
{
    public string Username { get; set; } = string.Empty;
    public Vector3 Position { get; set; }
    public Vector3 Velocity { get; set; }
    public float Health { get; set; }
    public float MaxHealth { get; set; }
    public int Level { get; set; }
    public TcpClient? Client { get; set; }
}

class Program
{
    private static bool _running = true;
    private static TcpListener? _listener;
    private static readonly ConcurrentDictionary<string, Player> _players = new();
    private static readonly int _tickRate = 60;
    private static readonly float _tickInterval = 1.0f / _tickRate;
    
    static async Task Main(string[] args)
    {
        Console.WriteLine("==================================");
        Console.WriteLine("  CloneMine Game Server (.NET 10)");
        Console.WriteLine("==================================");
        Console.WriteLine();
        
        // Parse command line arguments
        int port = 25565; // Default game port
        string configFile = "server_config.txt";
        
        if (args.Length > 0 && int.TryParse(args[0], out int parsedPort))
        {
            port = parsedPort;
        }
        
        if (args.Length > 1)
        {
            configFile = args[1];
        }
        
        Console.WriteLine("[GameServer] Configuration:");
        Console.WriteLine($"  Port: {port}");
        Console.WriteLine($"  Tick rate: {_tickRate} Hz");
        Console.WriteLine($"  Config file: {configFile}");
        Console.WriteLine();
        
        // Setup signal handlers
        Console.CancelKeyPress += (sender, e) =>
        {
            e.Cancel = true;
            Console.WriteLine("\n[GameServer] Shutdown signal received");
            _running = false;
        };
        
        try
        {
            // Start TCP listener
            _listener = new TcpListener(IPAddress.Any, port);
            _listener.Start();
            
            Console.WriteLine($"[GameServer] Started on port {port}");
            Console.WriteLine("[GameServer] Press Ctrl+C to stop");
            Console.WriteLine();
            
            // Accept connections asynchronously
            _ = Task.Run(AcceptClientsAsync);
            
            // Start game loop
            _ = Task.Run(GameLoopAsync);
            
            // Main loop
            while (_running)
            {
                await Task.Delay(100);
            }
        }
        catch (Exception ex)
        {
            Console.WriteLine($"[GameServer] Fatal error: {ex.Message}");
            return;
        }
        finally
        {
            // Shutdown
            Console.WriteLine("[GameServer] Shutting down...");
            _listener?.Stop();
            
            // Disconnect all players
            foreach (var player in _players.Values)
            {
                player.Client?.Close();
            }
            
            Console.WriteLine($"[GameServer] Final stats:");
            Console.WriteLine($"  Total players served: {_players.Count}");
            Console.WriteLine("[GameServer] Shutdown complete");
        }
    }
    
    private static async Task AcceptClientsAsync()
    {
        while (_running && _listener != null)
        {
            try
            {
                var client = await _listener.AcceptTcpClientAsync();
                Console.WriteLine($"[GameServer] Client connected: {client.Client.RemoteEndPoint}");
                
                // Handle client in separate task
                _ = Task.Run(() => HandleClientAsync(client));
            }
            catch (Exception ex) when (_running)
            {
                Console.WriteLine($"[GameServer] Error accepting client: {ex.Message}");
            }
        }
    }
    
    private static async Task HandleClientAsync(TcpClient client)
    {
        Player? player = null;
        
        try
        {
            using var stream = client.GetStream();
            var buffer = new byte[2048];
            
            while (_running && client.Connected)
            {
                var bytesRead = await stream.ReadAsync(buffer, 0, buffer.Length);
                if (bytesRead == 0) break;
                
                var message = Encoding.UTF8.GetString(buffer, 0, bytesRead).Trim();
                var response = ProcessMessage(message, client, ref player);
                
                if (!string.IsNullOrEmpty(response))
                {
                    var responseBytes = Encoding.UTF8.GetBytes(response + "\n");
                    await stream.WriteAsync(responseBytes, 0, responseBytes.Length);
                }
            }
        }
        catch (Exception ex)
        {
            Console.WriteLine($"[GameServer] Client error: {ex.Message}");
        }
        finally
        {
            if (player != null)
            {
                _players.TryRemove(player.Username, out _);
                Console.WriteLine($"[GameServer] Player disconnected: {player.Username}");
                BroadcastMessage($"PLAYER_LEFT|{player.Username}");
            }
            
            client.Close();
        }
    }
    
    private static string ProcessMessage(string message, TcpClient client, ref Player? player)
    {
        var parts = message.Split('|');
        if (parts.Length < 1) return "ERROR|Invalid message format";
        
        var command = parts[0].ToUpper();
        
        return command switch
        {
            "CONNECT" when parts.Length >= 2 => ProcessConnect(parts[1], client, ref player),
            "MOVE" when parts.Length >= 4 && player != null => ProcessMove(player, parts[1], parts[2], parts[3]),
            "GET_WORLD_STATE" => ProcessGetWorldState(),
            "ATTACK" when parts.Length >= 2 && player != null => ProcessAttack(player, parts[1]),
            "INTERACT" when parts.Length >= 4 && player != null => ProcessInteract(player, parts[1], parts[2], parts[3]),
            _ => "ERROR|Unknown command or not connected"
        };
    }
    
    private static string ProcessConnect(string username, TcpClient client, ref Player? player)
    {
        if (_players.ContainsKey(username))
        {
            return "ERROR|Username already connected";
        }
        
        player = new Player
        {
            Username = username,
            Position = new Vector3(0, 64, 0),
            Velocity = Vector3.Zero,
            Health = 100.0f,
            MaxHealth = 100.0f,
            Level = 1,
            Client = client
        };
        
        _players[username] = player;
        
        Console.WriteLine($"[GameServer] Player connected: {username} at {player.Position}");
        BroadcastMessage($"PLAYER_JOINED|{username}");
        
        return $"CONNECTED|{player.Position.X}|{player.Position.Y}|{player.Position.Z}";
    }
    
    private static string ProcessMove(Player player, string xStr, string yStr, string zStr)
    {
        if (float.TryParse(xStr, out float x) && 
            float.TryParse(yStr, out float y) && 
            float.TryParse(zStr, out float z))
        {
            player.Position = new Vector3(x, y, z);
            return $"MOVE_OK|{x}|{y}|{z}";
        }
        
        return "ERROR|Invalid position";
    }
    
    private static string ProcessGetWorldState()
    {
        var playerList = string.Join(";", _players.Values.Select(p =>
            $"{p.Username}:{p.Position.X}:{p.Position.Y}:{p.Position.Z}:{p.Health}:{p.Level}"));
        
        return $"WORLD_STATE|{_players.Count}|{playerList}";
    }
    
    private static string ProcessAttack(Player player, string targetUsername)
    {
        if (_players.TryGetValue(targetUsername, out var target))
        {
            var distance = Vector3.Distance(player.Position, target.Position);
            
            if (distance > 5.0f)
            {
                return "ERROR|Target too far away";
            }
            
            var damage = 10.0f;
            target.Health -= damage;
            
            if (target.Health <= 0)
            {
                target.Health = 0;
                Console.WriteLine($"[GameServer] {targetUsername} was defeated by {player.Username}");
                BroadcastMessage($"PLAYER_DEFEATED|{targetUsername}|{player.Username}");
            }
            
            return $"ATTACK_SUCCESS|{targetUsername}|{damage}|{target.Health}";
        }
        
        return "ERROR|Target not found";
    }
    
    private static string ProcessInteract(Player player, string xStr, string yStr, string zStr)
    {
        if (float.TryParse(xStr, out float x) && 
            float.TryParse(yStr, out float y) && 
            float.TryParse(zStr, out float z))
        {
            var interactPos = new Vector3(x, y, z);
            var distance = Vector3.Distance(player.Position, interactPos);
            
            if (distance > 5.0f)
            {
                return "ERROR|Too far away";
            }
            
            Console.WriteLine($"[GameServer] {player.Username} interacted at {interactPos}");
            return $"INTERACT_SUCCESS|{x}|{y}|{z}";
        }
        
        return "ERROR|Invalid position";
    }
    
    private static void BroadcastMessage(string message)
    {
        var messageBytes = Encoding.UTF8.GetBytes(message + "\n");
        
        foreach (var player in _players.Values)
        {
            try
            {
                if (player.Client != null && player.Client.Connected)
                {
                    var stream = player.Client.GetStream();
                    stream.Write(messageBytes, 0, messageBytes.Length);
                }
            }
            catch (Exception)
            {
                // Player disconnected, will be handled by their handler
            }
        }
    }
    
    private static async Task GameLoopAsync()
    {
        var lastUpdate = DateTime.Now;
        
        while (_running)
        {
            var now = DateTime.Now;
            var deltaTime = (float)(now - lastUpdate).TotalSeconds;
            
            if (deltaTime >= _tickInterval)
            {
                lastUpdate = now;
                
                // Update game state
                UpdateGameState(deltaTime);
                
                // Send state updates to all players
                if (_players.Count > 0)
                {
                    var stateMessage = ProcessGetWorldState();
                    BroadcastMessage(stateMessage);
                }
            }
            
            await Task.Delay(1); // Yield to other tasks
        }
    }
    
    private static void UpdateGameState(float deltaTime)
    {
        // Update player physics, AI, etc.
        foreach (var player in _players.Values)
        {
            // Apply gravity if player is in the air
            if (player.Position.Y > 64.0f)
            {
                player.Velocity = new Vector3(
                    player.Velocity.X,
                    player.Velocity.Y - 9.8f * deltaTime,
                    player.Velocity.Z
                );
                
                player.Position += player.Velocity * deltaTime;
                
                // Ground check
                if (player.Position.Y <= 64.0f)
                {
                    player.Position = new Vector3(player.Position.X, 64.0f, player.Position.Z);
                    player.Velocity = new Vector3(player.Velocity.X, 0, player.Velocity.Z);
                }
            }
        }
    }
}
