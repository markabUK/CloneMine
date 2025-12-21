using CloneMine.ChatServer.Interfaces;

namespace CloneMine.ChatServer.Handlers;

public class ChatMessageHandler : IMessageHandler
{
    private readonly IChatService _chatService;
    private readonly IInputValidator _validator;
    private Action<string>? _broadcastAction;

    public ChatMessageHandler(IChatService chatService, IInputValidator validator)
    {
        _chatService = chatService ?? throw new ArgumentNullException(nameof(chatService));
        _validator = validator ?? throw new ArgumentNullException(nameof(validator));
    }

    public void SetBroadcastAction(Action<string> broadcastAction)
    {
        _broadcastAction = broadcastAction;
    }

    public async Task<string> HandleMessageAsync(string message)
    {
        var parts = message.Split('|', 3);
        if (parts.Length < 2) return "ERROR|Invalid message format";

        var command = parts[0].ToUpperInvariant();

        return command switch
        {
            "JOIN" => await HandleJoinAsync(parts[1]),
            "CHAT" when parts.Length >= 3 => await HandleChatAsync(parts[1], parts[2]),
            "HISTORY" => await HandleHistoryAsync(),
            "PING" => "PONG",
            _ => "ERROR|Unknown command"
        };
    }

    private Task<string> HandleJoinAsync(string username)
    {
        var validation = _validator.ValidateUsername(username);
        if (!validation.IsValid) return Task.FromResult($"ERROR|{validation.ErrorMessage}");

        Console.WriteLine($"[ChatServer] {username} joined");
        _broadcastAction?.Invoke($"SYSTEM|{username} joined the chat");

        return Task.FromResult("SUCCESS|Joined chat");
    }

    private async Task<string> HandleChatAsync(string username, string message)
    {
        var result = await _chatService.SendMessageAsync(username, message);
        if (!result.Success) return $"ERROR|{result.ErrorMessage}";

        _broadcastAction?.Invoke($"CHAT|{username}|{message}");
        return "SUCCESS|Message sent";
    }

    private async Task<string> HandleHistoryAsync()
    {
        var history = await _chatService.GetHistoryAsync();
        var messages = history.ToList();

        if (messages.Count == 0) return "HISTORY|NONE";

        var historyStr = string.Join("|", messages.Select(m => $"{m.Username}:{m.Message}"));
        return $"HISTORY|{historyStr}";
    }
}
