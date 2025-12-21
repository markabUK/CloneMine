using CloneMine.QuestServer.Models;

namespace CloneMine.QuestServer.Interfaces;

public interface IQuestRepository
{
    Task<Quest?> GetQuestAsync(int questId);
    Task<IEnumerable<Quest>> GetAllQuestsAsync();
    Task<IEnumerable<Quest>> GetAvailableQuestsAsync(int playerLevel);
    Task<bool> AddQuestAsync(Quest quest);
}
