using System.Numerics;
using CloneMine.GameServer.Interfaces;
using CloneMine.GameServer.Models;

namespace CloneMine.GameServer.Services;

public class GameService : IGameService
{
    private readonly IPlayerRepository _repository;
    private readonly IInputValidator _validator;

    public GameService(IPlayerRepository repository, IInputValidator validator)
    {
        _repository = repository ?? throw new ArgumentNullException(nameof(repository));
        _validator = validator ?? throw new ArgumentNullException(nameof(validator));
    }

    public async Task<(bool Success, string Message)> JoinGameAsync(string username)
    {
        var validation = _validator.ValidateUsername(username);
        if (!validation.IsValid) return (false, validation.ErrorMessage);

        var existingPlayer = await _repository.GetPlayerAsync(username);
        if (existingPlayer != null) return (false, "Player already in game");

        var player = new Player
        {
            Username = username,
            Position = new Vector3(0, 64, 0),
            Velocity = Vector3.Zero,
            Health = 100.0f,
            MaxHealth = 100.0f,
            Level = 1
        };

        var added = await _repository.AddPlayerAsync(player);
        if (!added) return (false, "Failed to add player");

        Console.WriteLine($"[GameServer] Player joined: {username}");
        return (true, $"Joined game at position {player.Position}");
    }

    public async Task<(bool Success, string Message)> LeaveGameAsync(string username)
    {
        var validation = _validator.ValidateUsername(username);
        if (!validation.IsValid) return (false, validation.ErrorMessage);

        var removed = await _repository.RemovePlayerAsync(username);
        if (!removed) return (false, "Player not found");

        Console.WriteLine($"[GameServer] Player left: {username}");
        return (true, "Left game");
    }

    public Task<Player?> GetPlayerAsync(string username)
    {
        return _repository.GetPlayerAsync(username);
    }

    public Task<IEnumerable<Player>> GetAllPlayersAsync()
    {
        return _repository.GetAllPlayersAsync();
    }

    public async Task UpdatePlayerPositionAsync(string username, float x, float y, float z)
    {
        var player = await _repository.GetPlayerAsync(username);
        if (player == null) return;

        player.Position = new Vector3(x, y, z);
        await _repository.UpdatePlayerAsync(player);
    }

    public async Task UpdateGameStateAsync()
    {
        var players = await _repository.GetAllPlayersAsync();

        foreach (var player in players)
        {
            player.Position += player.Velocity * 0.016f;
            await _repository.UpdatePlayerAsync(player);
        }
    }
}
