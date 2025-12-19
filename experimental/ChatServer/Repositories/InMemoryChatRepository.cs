using CloneMine.ChatServer.Interfaces;
using CloneMine.ChatServer.Models;

namespace CloneMine.ChatServer.Repositories;

public class InMemoryChatRepository : IChatRepository
{
    private readonly List<ChatMessage> _history;
    private readonly int _maxHistory;
    private readonly object _lock = new();

    public InMemoryChatRepository(int maxHistory)
    {
        _history = new List<ChatMessage>();
        _maxHistory = maxHistory;
    }

    public Task AddMessageAsync(ChatMessage message)
    {
        if (message == null) throw new ArgumentNullException(nameof(message));

        lock (_lock)
        {
            _history.Add(message);
            if (_history.Count > _maxHistory)
            {
                _history.RemoveAt(0);
            }
        }

        return Task.CompletedTask;
    }

    public Task<IEnumerable<ChatMessage>> GetHistoryAsync()
    {
        lock (_lock)
        {
            return Task.FromResult<IEnumerable<ChatMessage>>(_history.ToList());
        }
    }
}
