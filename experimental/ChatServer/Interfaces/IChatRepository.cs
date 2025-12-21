using CloneMine.ChatServer.Models;

namespace CloneMine.ChatServer.Interfaces;

public interface IChatRepository
{
    Task AddMessageAsync(ChatMessage message);
    Task<IEnumerable<ChatMessage>> GetHistoryAsync();
}
