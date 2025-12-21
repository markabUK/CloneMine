using CloneMine.GameServer.Models;

namespace CloneMine.GameServer.Interfaces;

public interface IGameService
{
    Task<(bool Success, string Message)> JoinGameAsync(string username);
    Task<(bool Success, string Message)> LeaveGameAsync(string username);
    Task<Player?> GetPlayerAsync(string username);
    Task<IEnumerable<Player>> GetAllPlayersAsync();
    Task UpdatePlayerPositionAsync(string username, float x, float y, float z);
    Task UpdateGameStateAsync();
}
