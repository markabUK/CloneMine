namespace CloneMine.QuestServer.Models;

/// <summary>
/// Represents player progress on a quest
/// </summary>
public class PlayerQuestProgress
{
    public int QuestId { get; set; }
    public Dictionary<string, int> Progress { get; set; } = new();
    public bool IsCompleted { get; set; }
}
