using CloneMine.QuestServer.Interfaces;
using CloneMine.QuestServer.Models;

namespace CloneMine.QuestServer.Services;

public class QuestService : IQuestService
{
    private readonly IQuestRepository _questRepository;
    private readonly IProgressRepository _progressRepository;
    private readonly IInputValidator _validator;

    public QuestService(
        IQuestRepository questRepository,
        IProgressRepository progressRepository,
        IInputValidator validator)
    {
        _questRepository = questRepository ?? throw new ArgumentNullException(nameof(questRepository));
        _progressRepository = progressRepository ?? throw new ArgumentNullException(nameof(progressRepository));
        _validator = validator ?? throw new ArgumentNullException(nameof(validator));
    }

    public Task<IEnumerable<Quest>> GetAvailableQuestsAsync(int playerLevel)
    {
        return _questRepository.GetAvailableQuestsAsync(playerLevel);
    }

    public async Task<(bool Success, string Message)> AcceptQuestAsync(string username, int questId)
    {
        var validation = _validator.ValidateUsername(username);
        if (!validation.IsValid) return (false, validation.ErrorMessage);

        var quest = await _questRepository.GetQuestAsync(questId);
        if (quest == null) return (false, "Quest not found");

        var existingProgress = await _progressRepository.GetProgressAsync(username, questId);
        if (existingProgress != null) return (false, "Quest already accepted");

        var progress = new PlayerQuestProgress
        {
            QuestId = questId,
            Progress = new Dictionary<string, int>(),
            IsCompleted = false
        };

        await _progressRepository.SaveProgressAsync(username, progress);
        Console.WriteLine($"[QuestServer] {username} accepted quest: {quest.Title}");

        return (true, $"Accepted quest: {quest.Title}");
    }

    public async Task<(bool Success, string Message)> UpdateProgressAsync(string username, int questId, string objective, int amount)
    {
        var validation = _validator.ValidateUsername(username);
        if (!validation.IsValid) return (false, validation.ErrorMessage);

        var progress = await _progressRepository.GetProgressAsync(username, questId);
        if (progress == null) return (false, "Quest not accepted");

        if (progress.IsCompleted) return (false, "Quest already completed");

        if (!progress.Progress.ContainsKey(objective))
            progress.Progress[objective] = 0;

        progress.Progress[objective] += amount;

        await _progressRepository.SaveProgressAsync(username, progress);
        Console.WriteLine($"[QuestServer] {username} updated quest {questId}: {objective} += {amount}");

        return (true, $"Progress updated: {objective} = {progress.Progress[objective]}");
    }

    public async Task<(bool Success, string Message)> CompleteQuestAsync(string username, int questId)
    {
        var validation = _validator.ValidateUsername(username);
        if (!validation.IsValid) return (false, validation.ErrorMessage);

        var progress = await _progressRepository.GetProgressAsync(username, questId);
        if (progress == null) return (false, "Quest not accepted");

        if (progress.IsCompleted) return (false, "Quest already completed");

        var quest = await _questRepository.GetQuestAsync(questId);
        if (quest == null) return (false, "Quest not found");

        foreach (var objective in quest.Objectives)
        {
            if (!progress.Progress.TryGetValue(objective.Key, out int completed) || completed < objective.Value)
                return (false, $"Objective not complete: {objective.Key} ({completed}/{objective.Value})");
        }

        progress.IsCompleted = true;
        await _progressRepository.SaveProgressAsync(username, progress);

        Console.WriteLine($"[QuestServer] {username} completed quest: {quest.Title}");
        return (true, $"Quest completed! Rewards: {string.Join(", ", quest.Rewards.Select(r => $"{r.Value} {r.Key}"))}");
    }

    public Task<IEnumerable<PlayerQuestProgress>> GetPlayerProgressAsync(string username)
    {
        var validation = _validator.ValidateUsername(username);
        if (!validation.IsValid) return Task.FromResult<IEnumerable<PlayerQuestProgress>>(Array.Empty<PlayerQuestProgress>());

        return _progressRepository.GetAllProgressAsync(username);
    }
}
