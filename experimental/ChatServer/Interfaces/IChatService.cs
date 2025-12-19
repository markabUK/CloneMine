using CloneMine.ChatServer.Models;

namespace CloneMine.ChatServer.Interfaces;

public interface IChatService
{
    Task<(bool Success, string ErrorMessage)> SendMessageAsync(string username, string message);
    Task<IEnumerable<ChatMessage>> GetHistoryAsync();
}
