using CloneMine.QuestServer.Models;

namespace CloneMine.QuestServer.Interfaces;

public interface IQuestService
{
    Task<IEnumerable<Quest>> GetAvailableQuestsAsync(int playerLevel);
    Task<(bool Success, string Message)> AcceptQuestAsync(string username, int questId);
    Task<(bool Success, string Message)> UpdateProgressAsync(string username, int questId, string objective, int amount);
    Task<(bool Success, string Message)> CompleteQuestAsync(string username, int questId);
    Task<IEnumerable<PlayerQuestProgress>> GetPlayerProgressAsync(string username);
}
