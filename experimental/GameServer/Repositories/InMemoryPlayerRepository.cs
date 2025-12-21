using System.Collections.Concurrent;
using CloneMine.GameServer.Interfaces;
using CloneMine.GameServer.Models;

namespace CloneMine.GameServer.Repositories;

public class InMemoryPlayerRepository : IPlayerRepository
{
    private readonly ConcurrentDictionary<string, Player> _players;

    public InMemoryPlayerRepository()
    {
        _players = new ConcurrentDictionary<string, Player>(StringComparer.OrdinalIgnoreCase);
    }

    public Task<Player?> GetPlayerAsync(string username)
    {
        _players.TryGetValue(username, out var player);
        return Task.FromResult(player);
    }

    public Task<IEnumerable<Player>> GetAllPlayersAsync()
    {
        return Task.FromResult<IEnumerable<Player>>(_players.Values.ToList());
    }

    public Task<bool> AddPlayerAsync(Player player)
    {
        if (player == null) throw new ArgumentNullException(nameof(player));
        return Task.FromResult(_players.TryAdd(player.Username, player));
    }

    public Task<bool> UpdatePlayerAsync(Player player)
    {
        if (player == null) throw new ArgumentNullException(nameof(player));
        _players[player.Username] = player;
        return Task.FromResult(true);
    }

    public Task<bool> RemovePlayerAsync(string username)
    {
        return Task.FromResult(_players.TryRemove(username, out _));
    }
}
