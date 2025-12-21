using CloneMine.QuestServer.Interfaces;
using CloneMine.QuestServer.Models;

namespace CloneMine.QuestServer.Repositories;

public class InMemoryQuestRepository : IQuestRepository
{
    private readonly List<Quest> _quests;
    private readonly object _lock = new();

    public InMemoryQuestRepository()
    {
        _quests = new List<Quest>();
        InitializeDefaultQuests();
    }

    private void InitializeDefaultQuests()
    {
        _quests.Add(new Quest
        {
            QuestId = 1,
            Title = "First Steps",
            Description = "Kill 10 zombies to prove your worth",
            Type = QuestType.KillMonsters,
            MinLevel = 1,
            RecommendedLevel = 1,
            Objectives = new Dictionary<string, int> { { "zombies", 10 } },
            Rewards = new Dictionary<string, int> { { "gold", 50 }, { "experience", 100 } }
        });

        _quests.Add(new Quest
        {
            QuestId = 2,
            Title = "Resource Gathering",
            Description = "Collect 20 wood logs",
            Type = QuestType.CollectItems,
            MinLevel = 1,
            RecommendedLevel = 2,
            Objectives = new Dictionary<string, int> { { "wood", 20 } },
            Rewards = new Dictionary<string, int> { { "gold", 100 }, { "experience", 200 } }
        });
    }

    public Task<Quest?> GetQuestAsync(int questId)
    {
        lock (_lock)
        {
            var quest = _quests.FirstOrDefault(q => q.QuestId == questId);
            return Task.FromResult(quest);
        }
    }

    public Task<IEnumerable<Quest>> GetAllQuestsAsync()
    {
        lock (_lock)
        {
            return Task.FromResult<IEnumerable<Quest>>(_quests.ToList());
        }
    }

    public Task<IEnumerable<Quest>> GetAvailableQuestsAsync(int playerLevel)
    {
        lock (_lock)
        {
            var available = _quests.Where(q => q.MinLevel <= playerLevel).ToList();
            return Task.FromResult<IEnumerable<Quest>>(available);
        }
    }

    public Task<bool> AddQuestAsync(Quest quest)
    {
        if (quest == null) throw new ArgumentNullException(nameof(quest));

        lock (_lock)
        {
            if (_quests.Any(q => q.QuestId == quest.QuestId))
                return Task.FromResult(false);

            _quests.Add(quest);
            return Task.FromResult(true);
        }
    }
}
