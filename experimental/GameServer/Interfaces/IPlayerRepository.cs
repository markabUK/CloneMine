using CloneMine.GameServer.Models;

namespace CloneMine.GameServer.Interfaces;

public interface IPlayerRepository
{
    Task<Player?> GetPlayerAsync(string username);
    Task<IEnumerable<Player>> GetAllPlayersAsync();
    Task<bool> AddPlayerAsync(Player player);
    Task<bool> UpdatePlayerAsync(Player player);
    Task<bool> RemovePlayerAsync(string username);
}
