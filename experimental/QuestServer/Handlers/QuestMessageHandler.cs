using CloneMine.QuestServer.Interfaces;

namespace CloneMine.QuestServer.Handlers;

public class QuestMessageHandler : IMessageHandler
{
    private readonly IQuestService _questService;
    private readonly IInputValidator _validator;

    public QuestMessageHandler(IQuestService questService, IInputValidator validator)
    {
        _questService = questService ?? throw new ArgumentNullException(nameof(questService));
        _validator = validator ?? throw new ArgumentNullException(nameof(validator));
    }

    public async Task<string> HandleMessageAsync(string message)
    {
        var validation = _validator.ValidateMessage(message);
        if (!validation.IsValid) return $"ERROR|{validation.ErrorMessage}";

        var parts = message.Split('|');
        if (parts.Length < 1) return "ERROR|Invalid message format";

        var command = parts[0].ToUpperInvariant();

        return command switch
        {
            "GET_QUESTS" when parts.Length >= 2 => await HandleGetQuestsAsync(parts[1]),
            "ACCEPT_QUEST" when parts.Length >= 3 => await HandleAcceptQuestAsync(parts[1], parts[2]),
            "UPDATE_PROGRESS" when parts.Length >= 5 => await HandleUpdateProgressAsync(parts[1], parts[2], parts[3], parts[4]),
            "COMPLETE_QUEST" when parts.Length >= 3 => await HandleCompleteQuestAsync(parts[1], parts[2]),
            "GET_PROGRESS" when parts.Length >= 2 => await HandleGetProgressAsync(parts[1]),
            "PING" => "PONG",
            _ => "ERROR|Unknown command"
        };
    }

    private async Task<string> HandleGetQuestsAsync(string levelStr)
    {
        if (!int.TryParse(levelStr, out int level))
            return "ERROR|Invalid level";

        var quests = await _questService.GetAvailableQuestsAsync(level);
        var questList = quests.ToList();

        if (questList.Count == 0) return "QUESTS|NONE";

        var list = string.Join(";", questList.Select(q =>
            $"{q.QuestId}:{q.Title}:{q.Type}:{q.MinLevel}"));

        return $"QUESTS|{list}";
    }

    private async Task<string> HandleAcceptQuestAsync(string username, string questIdStr)
    {
        if (!int.TryParse(questIdStr, out int questId))
            return "ERROR|Invalid quest ID";

        var result = await _questService.AcceptQuestAsync(username, questId);
        return result.Success ? $"SUCCESS|{result.Message}" : $"ERROR|{result.Message}";
    }

    private async Task<string> HandleUpdateProgressAsync(string username, string questIdStr, string objective, string amountStr)
    {
        if (!int.TryParse(questIdStr, out int questId))
            return "ERROR|Invalid quest ID";

        if (!int.TryParse(amountStr, out int amount))
            return "ERROR|Invalid amount";

        var result = await _questService.UpdateProgressAsync(username, questId, objective, amount);
        return result.Success ? $"SUCCESS|{result.Message}" : $"ERROR|{result.Message}";
    }

    private async Task<string> HandleCompleteQuestAsync(string username, string questIdStr)
    {
        if (!int.TryParse(questIdStr, out int questId))
            return "ERROR|Invalid quest ID";

        var result = await _questService.CompleteQuestAsync(username, questId);
        return result.Success ? $"SUCCESS|{result.Message}" : $"ERROR|{result.Message}";
    }

    private async Task<string> HandleGetProgressAsync(string username)
    {
        var progress = await _questService.GetPlayerProgressAsync(username);
        var progressList = progress.ToList();

        if (progressList.Count == 0) return "PROGRESS|NONE";

        var list = string.Join(";", progressList.Select(p =>
            $"{p.QuestId}:{p.IsCompleted}:{string.Join(",", p.Progress.Select(pr => $"{pr.Key}={pr.Value}"))}"));

        return $"PROGRESS|{list}";
    }
}
