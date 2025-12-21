using CloneMine.ChatServer.Interfaces;
using CloneMine.ChatServer.Models;

namespace CloneMine.ChatServer.Services;

public class ChatService : IChatService
{
    private readonly IChatRepository _repository;
    private readonly IInputValidator _validator;

    public ChatService(IChatRepository repository, IInputValidator validator)
    {
        _repository = repository ?? throw new ArgumentNullException(nameof(repository));
        _validator = validator ?? throw new ArgumentNullException(nameof(validator));
    }

    public async Task<(bool Success, string ErrorMessage)> SendMessageAsync(string username, string message)
    {
        var usernameValidation = _validator.ValidateUsername(username);
        if (!usernameValidation.IsValid) return (false, usernameValidation.ErrorMessage);

        var messageValidation = _validator.ValidateMessage(message);
        if (!messageValidation.IsValid) return (false, messageValidation.ErrorMessage);

        var chatMessage = new ChatMessage
        {
            Username = username,
            Message = message,
            Timestamp = DateTime.UtcNow
        };

        await _repository.AddMessageAsync(chatMessage);
        Console.WriteLine($"[ChatServer] {username}: {message}");

        return (true, string.Empty);
    }

    public Task<IEnumerable<ChatMessage>> GetHistoryAsync()
    {
        return _repository.GetHistoryAsync();
    }
}
