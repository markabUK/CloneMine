using CloneMine.QuestServer.Models;

namespace CloneMine.QuestServer.Interfaces;

public interface IProgressRepository
{
    Task<PlayerQuestProgress?> GetProgressAsync(string username, int questId);
    Task<IEnumerable<PlayerQuestProgress>> GetAllProgressAsync(string username);
    Task<bool> SaveProgressAsync(string username, PlayerQuestProgress progress);
}
