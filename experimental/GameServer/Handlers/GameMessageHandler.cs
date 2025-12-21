using CloneMine.GameServer.Interfaces;

namespace CloneMine.GameServer.Handlers;

public class GameMessageHandler : IMessageHandler
{
    private readonly IGameService _gameService;
    private readonly IInputValidator _validator;

    public GameMessageHandler(IGameService gameService, IInputValidator validator)
    {
        _gameService = gameService ?? throw new ArgumentNullException(nameof(gameService));
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
            "JOIN" when parts.Length >= 2 => await HandleJoinAsync(parts[1]),
            "LEAVE" when parts.Length >= 2 => await HandleLeaveAsync(parts[1]),
            "GET_PLAYERS" => await HandleGetPlayersAsync(),
            "GET_PLAYER" when parts.Length >= 2 => await HandleGetPlayerAsync(parts[1]),
            "UPDATE_POS" when parts.Length >= 5 => await HandleUpdatePositionAsync(parts[1], parts[2], parts[3], parts[4]),
            "PING" => "PONG",
            _ => "ERROR|Unknown command"
        };
    }

    private async Task<string> HandleJoinAsync(string username)
    {
        var result = await _gameService.JoinGameAsync(username);
        return result.Success ? $"SUCCESS|{result.Message}" : $"ERROR|{result.Message}";
    }

    private async Task<string> HandleLeaveAsync(string username)
    {
        var result = await _gameService.LeaveGameAsync(username);
        return result.Success ? $"SUCCESS|{result.Message}" : $"ERROR|{result.Message}";
    }

    private async Task<string> HandleGetPlayersAsync()
    {
        var players = await _gameService.GetAllPlayersAsync();
        var playerList = players.ToList();

        if (playerList.Count == 0) return "PLAYERS|NONE";

        var list = string.Join(";", playerList.Select(p =>
            $"{p.Username}:{p.Position.X},{p.Position.Y},{p.Position.Z}:{p.Health}"));

        return $"PLAYERS|{list}";
    }

    private async Task<string> HandleGetPlayerAsync(string username)
    {
        var player = await _gameService.GetPlayerAsync(username);
        if (player == null) return "ERROR|Player not found";

        var playerData = $"{player.Username}|{player.Position.X}|{player.Position.Y}|{player.Position.Z}|" +
                        $"{player.Health}|{player.MaxHealth}|{player.Level}";

        return $"PLAYER|{playerData}";
    }

    private async Task<string> HandleUpdatePositionAsync(string username, string xStr, string yStr, string zStr)
    {
        if (!float.TryParse(xStr, out float x) ||
            !float.TryParse(yStr, out float y) ||
            !float.TryParse(zStr, out float z))
        {
            return "ERROR|Invalid position values";
        }

        await _gameService.UpdatePlayerPositionAsync(username, x, y, z);
        return "SUCCESS|Position updated";
    }
}
