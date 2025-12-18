using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Collections.Concurrent;
using System.Text.Json;

namespace CloneMine.CharacterServer;

class Character
{
    public string Name { get; set; } = string.Empty;
    public string Owner { get; set; } = string.Empty;
    public int Level { get; set; }
    public int Experience { get; set; }
    public float Health { get; set; }
    public float MaxHealth { get; set; }
    public float PositionX { get; set; }
    public float PositionY { get; set; }
    public float PositionZ { get; set; }
}

class Program
{
    private static bool _running = true;
    private static TcpListener? _listener;
    private static readonly ConcurrentDictionary<string, List<Character>> _accountCharacters = new();
    private static int _maxCharactersPerAccount = 5;
    
    static async Task Main(string[] args)
    {
        Console.WriteLine("==================================");
        Console.WriteLine("  CloneMine Character Server (.NET 10)");
        Console.WriteLine("==================================");
        Console.WriteLine();
        
        // Parse command line arguments
        int port = 25568; // Default character port
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
        
        Console.WriteLine("[CharacterServer] Configuration:");
        Console.WriteLine($"  Port: {port}");
        Console.WriteLine($"  Max characters per account: {_maxCharactersPerAccount}");
        Console.WriteLine($"  Config file: {configFile}");
        Console.WriteLine();
        
        // Setup signal handlers
        Console.CancelKeyPress += (sender, e) =>
        {
            e.Cancel = true;
            Console.WriteLine("\n[CharacterServer] Shutdown signal received");
            _running = false;
        };
        
        try
        {
            // Start TCP listener
            _listener = new TcpListener(IPAddress.Any, port);
            _listener.Start();
            
            Console.WriteLine($"[CharacterServer] Started on port {port}");
            Console.WriteLine("[CharacterServer] Press Ctrl+C to stop");
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
            Console.WriteLine($"[CharacterServer] Fatal error: {ex.Message}");
            return;
        }
        finally
        {
            // Shutdown
            Console.WriteLine("[CharacterServer] Shutting down...");
            _listener?.Stop();
            Console.WriteLine("[CharacterServer] Shutdown complete");
        }
    }
    
    private static async Task AcceptClientsAsync()
    {
        while (_running && _listener != null)
        {
            try
            {
                var client = await _listener.AcceptTcpClientAsync();
                Console.WriteLine($"[CharacterServer] Client connected: {client.Client.RemoteEndPoint}");
                
                // Handle client in separate task
                _ = Task.Run(() => HandleClientAsync(client));
            }
            catch (Exception ex) when (_running)
            {
                Console.WriteLine($"[CharacterServer] Error accepting client: {ex.Message}");
            }
        }
    }
    
    private static async Task HandleClientAsync(TcpClient client)
    {
        try
        {
            using var stream = client.GetStream();
            var buffer = new byte[2048];
            
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
            Console.WriteLine($"[CharacterServer] Client error: {ex.Message}");
        }
        finally
        {
            client.Close();
        }
    }
    
    private static string ProcessMessage(string message)
    {
        var parts = message.Split('|');
        if (parts.Length < 1) return "ERROR|Invalid message format";
        
        var command = parts[0].ToUpper();
        
        return command switch
        {
            "LIST_CHARACTERS" when parts.Length >= 2 => ProcessListCharacters(parts[1]),
            "CREATE_CHARACTER" when parts.Length >= 3 => ProcessCreateCharacter(parts[1], parts[2]),
            "DELETE_CHARACTER" when parts.Length >= 3 => ProcessDeleteCharacter(parts[1], parts[2]),
            "GET_CHARACTER" when parts.Length >= 3 => ProcessGetCharacter(parts[1], parts[2]),
            "UPDATE_CHARACTER" when parts.Length >= 3 => ProcessUpdateCharacter(parts[1], parts[2]),
            _ => "ERROR|Unknown command"
        };
    }
    
    private static string ProcessListCharacters(string username)
    {
        if (!_accountCharacters.TryGetValue(username, out var characters))
        {
            return "CHARACTERS|NONE";
        }
        
        var charList = string.Join(";", characters.Select(c => 
            $"{c.Name}:{c.Level}:{c.Health}:{c.MaxHealth}"));
        
        return $"CHARACTERS|{charList}";
    }
    
    private static string ProcessCreateCharacter(string username, string characterName)
    {
        if (string.IsNullOrWhiteSpace(characterName) || characterName.Length < 3)
        {
            return "ERROR|Character name must be at least 3 characters";
        }
        
        if (characterName.Length > 16)
        {
            return "ERROR|Character name must be at most 16 characters";
        }
        
        var characters = _accountCharacters.GetOrAdd(username, new List<Character>());
        
        if (characters.Count >= _maxCharactersPerAccount)
        {
            return $"ERROR|Maximum {_maxCharactersPerAccount} characters per account";
        }
        
        if (characters.Any(c => c.Name.Equals(characterName, StringComparison.OrdinalIgnoreCase)))
        {
            return "ERROR|Character name already exists";
        }
        
        var newCharacter = new Character
        {
            Name = characterName,
            Owner = username,
            Level = 1,
            Experience = 0,
            Health = 100.0f,
            MaxHealth = 100.0f,
            PositionX = 0.0f,
            PositionY = 64.0f,
            PositionZ = 0.0f
        };
        
        lock (characters)
        {
            characters.Add(newCharacter);
        }
        
        Console.WriteLine($"[CharacterServer] Character created: {username}/{characterName}");
        return $"SUCCESS|Character created: {characterName}";
    }
    
    private static string ProcessDeleteCharacter(string username, string characterName)
    {
        if (!_accountCharacters.TryGetValue(username, out var characters))
        {
            return "ERROR|No characters found";
        }
        
        lock (characters)
        {
            var character = characters.FirstOrDefault(c => 
                c.Name.Equals(characterName, StringComparison.OrdinalIgnoreCase));
            
            if (character == null)
            {
                return "ERROR|Character not found";
            }
            
            characters.Remove(character);
        }
        
        Console.WriteLine($"[CharacterServer] Character deleted: {username}/{characterName}");
        return $"SUCCESS|Character deleted: {characterName}";
    }
    
    private static string ProcessGetCharacter(string username, string characterName)
    {
        if (!_accountCharacters.TryGetValue(username, out var characters))
        {
            return "ERROR|No characters found";
        }
        
        var character = characters.FirstOrDefault(c => 
            c.Name.Equals(characterName, StringComparison.OrdinalIgnoreCase));
        
        if (character == null)
        {
            return "ERROR|Character not found";
        }
        
        var charData = $"{character.Name}|{character.Level}|{character.Experience}|" +
                      $"{character.Health}|{character.MaxHealth}|" +
                      $"{character.PositionX}|{character.PositionY}|{character.PositionZ}";
        
        return $"CHARACTER|{charData}";
    }
    
    private static string ProcessUpdateCharacter(string username, string characterData)
    {
        var parts = characterData.Split('|');
        if (parts.Length < 8)
        {
            return "ERROR|Invalid character data";
        }
        
        var characterName = parts[0];
        
        if (!_accountCharacters.TryGetValue(username, out var characters))
        {
            return "ERROR|No characters found";
        }
        
        var character = characters.FirstOrDefault(c => 
            c.Name.Equals(characterName, StringComparison.OrdinalIgnoreCase));
        
        if (character == null)
        {
            return "ERROR|Character not found";
        }
        
        try
        {
            lock (characters)
            {
                character.Level = int.Parse(parts[1]);
                character.Experience = int.Parse(parts[2]);
                character.Health = float.Parse(parts[3]);
                character.MaxHealth = float.Parse(parts[4]);
                character.PositionX = float.Parse(parts[5]);
                character.PositionY = float.Parse(parts[6]);
                character.PositionZ = float.Parse(parts[7]);
            }
            
            Console.WriteLine($"[CharacterServer] Character updated: {username}/{characterName}");
            return $"SUCCESS|Character updated: {characterName}";
        }
        catch (Exception ex)
        {
            return $"ERROR|Failed to update character: {ex.Message}";
        }
    }
}
