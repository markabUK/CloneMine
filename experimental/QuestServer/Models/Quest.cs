namespace CloneMine.QuestServer.Models;

/// <summary>
/// Represents a quest
/// </summary>
public class Quest
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
