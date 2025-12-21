using System.Collections.Concurrent;
using CloneMine.QuestServer.Interfaces;
using CloneMine.QuestServer.Models;

namespace CloneMine.QuestServer.Repositories;

public class InMemoryProgressRepository : IProgressRepository
{
    private readonly ConcurrentDictionary<string, List<PlayerQuestProgress>> _playerProgress;

    public InMemoryProgressRepository()
    {
        _playerProgress = new ConcurrentDictionary<string, List<PlayerQuestProgress>>(StringComparer.OrdinalIgnoreCase);
    }

    public Task<PlayerQuestProgress?> GetProgressAsync(string username, int questId)
    {
        if (!_playerProgress.TryGetValue(username, out var progress))
            return Task.FromResult<PlayerQuestProgress?>(null);

        var questProgress = progress.FirstOrDefault(p => p.QuestId == questId);
        return Task.FromResult(questProgress);
    }

    public Task<IEnumerable<PlayerQuestProgress>> GetAllProgressAsync(string username)
    {
        if (!_playerProgress.TryGetValue(username, out var progress))
            return Task.FromResult<IEnumerable<PlayerQuestProgress>>(Array.Empty<PlayerQuestProgress>());

        return Task.FromResult<IEnumerable<PlayerQuestProgress>>(progress.ToList());
    }

    public Task<bool> SaveProgressAsync(string username, PlayerQuestProgress progress)
    {
        if (progress == null) throw new ArgumentNullException(nameof(progress));

        var playerProgress = _playerProgress.GetOrAdd(username, new List<PlayerQuestProgress>());

        lock (playerProgress)
        {
            var existing = playerProgress.FirstOrDefault(p => p.QuestId == progress.QuestId);
            if (existing != null)
            {
                playerProgress.Remove(existing);
            }
            playerProgress.Add(progress);
        }

        return Task.FromResult(true);
    }
}
