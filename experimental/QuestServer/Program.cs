using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Collections.Concurrent;
using System.Text.Json;

namespace CloneMine.QuestServer;

enum QuestType
{
    KillMonsters,
    CollectItems,
    ExploreLocation,
    TalkToNpc,
    CraftItem,
    Combination
}

class Quest
{
    public int QuestId { get; set; }
    public string Title { get; set; } = string.Empty;
    public string Description { get; set; } = string.Empty;
    public QuestType Type { get; set; }
    public int MinLevel { get; set; }
    public int RecommendedLevel { get; set; }
    public Dictionary<string, int> Objectives { get; set; } = new();
    public Dictionary<string, int> Rewards { get; set; } = new();
}

class PlayerQuestProgress
{
    public int QuestId { get; set; }
    public Dictionary<string, int> Progress { get; set; } = new();
    public bool IsCompleted { get; set; }
}

class Program
{
    private static bool _running = true;
    private static TcpListener? _listener;
    private static readonly List<Quest> _quests = new();
    private static readonly ConcurrentDictionary<string, List<PlayerQuestProgress>> _playerProgress = new();
    
    static async Task Main(string[] args)
    {
        Console.WriteLine("==================================");
        Console.WriteLine("  CloneMine Quest Server (.NET 10)");
        Console.WriteLine("==================================");
        Console.WriteLine();
        
        // Parse command line arguments
        int port = 25567; // Default quest port
        string configFile = "server_config.txt";
        
        if (args.Length > 0 && int.TryParse(args[0], out int parsedPort))
        {
            port = parsedPort;
        }
        
        if (args.Length > 1)
        {
            configFile = args[1];
        }
        
        // Initialize sample quests
        InitializeQuests();
        
        Console.WriteLine("[QuestServer] Configuration:");
        Console.WriteLine($"  Port: {port}");
        Console.WriteLine($"  Available quests: {_quests.Count}");
        Console.WriteLine($"  Config file: {configFile}");
        Console.WriteLine();
        
        // Setup signal handlers
        Console.CancelKeyPress += (sender, e) =>
        {
            e.Cancel = true;
            Console.WriteLine("\n[QuestServer] Shutdown signal received");
            _running = false;
        };
        
        try
        {
            // Start TCP listener
            _listener = new TcpListener(IPAddress.Any, port);
            _listener.Start();
            
            Console.WriteLine($"[QuestServer] Started on port {port}");
            Console.WriteLine("[QuestServer] Press Ctrl+C to stop");
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
            Console.WriteLine($"[QuestServer] Fatal error: {ex.Message}");
            return;
        }
        finally
        {
            // Shutdown
            Console.WriteLine("[QuestServer] Shutting down...");
            _listener?.Stop();
            Console.WriteLine("[QuestServer] Shutdown complete");
        }
    }
    
    private static void InitializeQuests()
    {
        // Quest 1: Zombie Slayer I
        _quests.Add(new Quest
        {
            QuestId = 1,
            Title = "Zombie Slayer I",
            Description = "Defeat 10 zombies to prove your combat skills",
            Type = QuestType.KillMonsters,
            MinLevel = 1,
            RecommendedLevel = 3,
            Objectives = new Dictionary<string, int> { { "zombies", 10 } },
            Rewards = new Dictionary<string, int> { { "xp", 100 }, { "gold", 50 } }
        });
        
        // Quest 2: Gather Resources
        _quests.Add(new Quest
        {
            QuestId = 2,
            Title = "Gather Resources",
            Description = "Collect 20 wood blocks for the village",
            Type = QuestType.CollectItems,
            MinLevel = 1,
            RecommendedLevel = 1,
            Objectives = new Dictionary<string, int> { { "wood_blocks", 20 } },
            Rewards = new Dictionary<string, int> { { "xp", 50 }, { "gold", 25 } }
        });
        
        // Quest 3: Dungeon Explorer
        _quests.Add(new Quest
        {
            QuestId = 3,
            Title = "Dungeon Explorer",
            Description = "Explore the Dark Cavern and defeat the Cave Troll",
            Type = QuestType.Combination,
            MinLevel = 5,
            RecommendedLevel = 8,
            Objectives = new Dictionary<string, int> 
            { 
                { "explore_dark_cavern", 1 },
                { "defeat_cave_troll", 1 }
            },
            Rewards = new Dictionary<string, int> { { "xp", 500 }, { "gold", 250 } }
        });
        
        Console.WriteLine("[QuestServer] Initialized quests:");
        foreach (var quest in _quests)
        {
            Console.WriteLine($"  {quest.QuestId}. {quest.Title} (Level {quest.MinLevel}+)");
        }
        Console.WriteLine();
    }
    
    private static async Task AcceptClientsAsync()
    {
        while (_running && _listener != null)
        {
            try
            {
                var client = await _listener.AcceptTcpClientAsync();
                Console.WriteLine($"[QuestServer] Client connected: {client.Client.RemoteEndPoint}");
                
                // Handle client in separate task
                _ = Task.Run(() => HandleClientAsync(client));
            }
            catch (Exception ex) when (_running)
            {
                Console.WriteLine($"[QuestServer] Error accepting client: {ex.Message}");
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
            Console.WriteLine($"[QuestServer] Client error: {ex.Message}");
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
            "LIST_QUESTS" when parts.Length >= 2 => ProcessListQuests(parts[1]),
            "GET_QUEST" when parts.Length >= 2 => ProcessGetQuest(parts[1]),
            "ACCEPT_QUEST" when parts.Length >= 3 => ProcessAcceptQuest(parts[1], parts[2]),
            "UPDATE_PROGRESS" when parts.Length >= 4 => ProcessUpdateProgress(parts[1], parts[2], parts[3]),
            "GET_PROGRESS" when parts.Length >= 2 => ProcessGetProgress(parts[1]),
            "COMPLETE_QUEST" when parts.Length >= 3 => ProcessCompleteQuest(parts[1], parts[2]),
            _ => "ERROR|Unknown command"
        };
    }
    
    private static string ProcessListQuests(string playerLevel)
    {
        if (!int.TryParse(playerLevel, out int level))
        {
            return "ERROR|Invalid level";
        }
        
        var availableQuests = _quests.Where(q => q.MinLevel <= level).ToList();
        var questList = string.Join(";", availableQuests.Select(q => 
            $"{q.QuestId}:{q.Title}:{q.MinLevel}:{q.RecommendedLevel}"));
        
        return $"QUESTS|{questList}";
    }
    
    private static string ProcessGetQuest(string questIdStr)
    {
        if (!int.TryParse(questIdStr, out int questId))
        {
            return "ERROR|Invalid quest ID";
        }
        
        var quest = _quests.FirstOrDefault(q => q.QuestId == questId);
        if (quest == null)
        {
            return "ERROR|Quest not found";
        }
        
        var objectives = string.Join(";", quest.Objectives.Select(o => $"{o.Key}:{o.Value}"));
        var rewards = string.Join(";", quest.Rewards.Select(r => $"{r.Key}:{r.Value}"));
        
        return $"QUEST|{quest.QuestId}|{quest.Title}|{quest.Description}|{objectives}|{rewards}";
    }
    
    private static string ProcessAcceptQuest(string username, string questIdStr)
    {
        if (!int.TryParse(questIdStr, out int questId))
        {
            return "ERROR|Invalid quest ID";
        }
        
        var quest = _quests.FirstOrDefault(q => q.QuestId == questId);
        if (quest == null)
        {
            return "ERROR|Quest not found";
        }
        
        var progress = new PlayerQuestProgress
        {
            QuestId = questId,
            Progress = quest.Objectives.ToDictionary(o => o.Key, o => 0),
            IsCompleted = false
        };
        
        _playerProgress.AddOrUpdate(username, 
            new List<PlayerQuestProgress> { progress },
            (key, list) => { list.Add(progress); return list; });
        
        Console.WriteLine($"[QuestServer] {username} accepted quest: {quest.Title}");
        return $"SUCCESS|Quest accepted: {quest.Title}";
    }
    
    private static string ProcessUpdateProgress(string username, string questIdStr, string objectiveData)
    {
        if (!int.TryParse(questIdStr, out int questId))
        {
            return "ERROR|Invalid quest ID";
        }
        
        if (!_playerProgress.TryGetValue(username, out var quests))
        {
            return "ERROR|No active quests";
        }
        
        var questProgress = quests.FirstOrDefault(q => q.QuestId == questId && !q.IsCompleted);
        if (questProgress == null)
        {
            return "ERROR|Quest not active";
        }
        
        var objParts = objectiveData.Split(':');
        if (objParts.Length != 2) return "ERROR|Invalid objective data";
        
        var objectiveName = objParts[0];
        if (!int.TryParse(objParts[1], out int amount))
        {
            return "ERROR|Invalid amount";
        }
        
        if (questProgress.Progress.ContainsKey(objectiveName))
        {
            questProgress.Progress[objectiveName] += amount;
            
            var quest = _quests.First(q => q.QuestId == questId);
            var required = quest.Objectives[objectiveName];
            var current = questProgress.Progress[objectiveName];
            
            Console.WriteLine($"[QuestServer] {username} progress: {quest.Title} - {objectiveName} {current}/{required}");
            
            return $"PROGRESS|{objectiveName}|{current}|{required}";
        }
        
        return "ERROR|Invalid objective";
    }
    
    private static string ProcessGetProgress(string username)
    {
        if (!_playerProgress.TryGetValue(username, out var quests))
        {
            return "PROGRESS|NONE";
        }
        
        var activeQuests = quests.Where(q => !q.IsCompleted).ToList();
        var progressList = string.Join(";", activeQuests.Select(q => 
        {
            var progress = string.Join(",", q.Progress.Select(p => $"{p.Key}:{p.Value}"));
            return $"{q.QuestId}:{progress}";
        }));
        
        return $"PROGRESS|{progressList}";
    }
    
    private static string ProcessCompleteQuest(string username, string questIdStr)
    {
        if (!int.TryParse(questIdStr, out int questId))
        {
            return "ERROR|Invalid quest ID";
        }
        
        if (!_playerProgress.TryGetValue(username, out var quests))
        {
            return "ERROR|No active quests";
        }
        
        var questProgress = quests.FirstOrDefault(q => q.QuestId == questId && !q.IsCompleted);
        if (questProgress == null)
        {
            return "ERROR|Quest not active";
        }
        
        var quest = _quests.First(q => q.QuestId == questId);
        
        // Check if all objectives are complete
        var allComplete = quest.Objectives.All(obj => 
            questProgress.Progress.TryGetValue(obj.Key, out int current) && current >= obj.Value);
        
        if (!allComplete)
        {
            return "ERROR|Quest objectives not complete";
        }
        
        questProgress.IsCompleted = true;
        
        var rewards = string.Join(";", quest.Rewards.Select(r => $"{r.Key}:{r.Value}"));
        Console.WriteLine($"[QuestServer] {username} completed quest: {quest.Title}");
        
        return $"COMPLETE|{quest.Title}|{rewards}";
    }
}
