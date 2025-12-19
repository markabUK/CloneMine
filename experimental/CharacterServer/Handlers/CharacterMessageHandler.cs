using CloneMine.CharacterServer.Interfaces;
using CloneMine.CharacterServer.Models;

namespace CloneMine.CharacterServer.Handlers;

public class CharacterMessageHandler : IMessageHandler
{
    private readonly ICharacterService _characterService;
    private readonly IInputValidator _validator;

    public CharacterMessageHandler(
        ICharacterService characterService,
        IInputValidator validator)
    {
        _characterService = characterService ?? throw new ArgumentNullException(nameof(characterService));
        _validator = validator ?? throw new ArgumentNullException(nameof(validator));
    }

    public async Task<string> HandleMessageAsync(string message)
    {
        var validation = _validator.ValidateMessage(message);
        if (!validation.IsValid)
            return $"ERROR|{validation.ErrorMessage}";

        var parts = message.Split('|', StringSplitOptions.None);
        if (parts.Length < 1) return "ERROR|Invalid message format";

        var command = parts[0]?.ToUpperInvariant();
        if (string.IsNullOrWhiteSpace(command)) return "ERROR|Command cannot be empty";

        return command switch
        {
            "LIST_CHARACTERS" => await HandleListCharactersAsync(parts),
            "CREATE_CHARACTER" => await HandleCreateCharacterAsync(parts),
            "DELETE_CHARACTER" => await HandleDeleteCharacterAsync(parts),
            "GET_CHARACTER" => await HandleGetCharacterAsync(parts),
            "UPDATE_CHARACTER" => await HandleUpdateCharacterAsync(parts),
            "PING" => "PONG",
            _ => "ERROR|Unknown command"
        };
    }

    private async Task<string> HandleListCharactersAsync(string[] parts)
    {
        if (parts.Length < 2)
            return "ERROR|Invalid LIST_CHARACTERS format. Expected: LIST_CHARACTERS|username";

        var username = parts[1];
        var characters = await _characterService.ListCharactersAsync(username);
        var charList = characters.ToList();

        if (charList.Count == 0)
            return "CHARACTERS|NONE";

        var list = string.Join(";", charList.Select(c =>
            $"{c.Name}:{c.Level}:{c.Health}:{c.MaxHealth}"));

        return $"CHARACTERS|{list}";
    }

    private async Task<string> HandleCreateCharacterAsync(string[] parts)
    {
        if (parts.Length < 3)
            return "ERROR|Invalid CREATE_CHARACTER format. Expected: CREATE_CHARACTER|username|characterName";

        var username = parts[1];
        var characterName = parts[2];

        var result = await _characterService.CreateCharacterAsync(username, characterName);
        return result.Success ? $"SUCCESS|{result.Message}" : $"ERROR|{result.Message}";
    }

    private async Task<string> HandleDeleteCharacterAsync(string[] parts)
    {
        if (parts.Length < 3)
            return "ERROR|Invalid DELETE_CHARACTER format. Expected: DELETE_CHARACTER|username|characterName";

        var username = parts[1];
        var characterName = parts[2];

        var result = await _characterService.DeleteCharacterAsync(username, characterName);
        return result.Success ? $"SUCCESS|{result.Message}" : $"ERROR|{result.Message}";
    }

    private async Task<string> HandleGetCharacterAsync(string[] parts)
    {
        if (parts.Length < 3)
            return "ERROR|Invalid GET_CHARACTER format. Expected: GET_CHARACTER|username|characterName";

        var username = parts[1];
        var characterName = parts[2];

        var character = await _characterService.GetCharacterAsync(username, characterName);
        if (character == null)
            return "ERROR|Character not found";

        var charData = $"{character.Name}|{character.Level}|{character.Experience}|" +
                      $"{character.Health}|{character.MaxHealth}|" +
                      $"{character.PositionX}|{character.PositionY}|{character.PositionZ}";

        return $"CHARACTER|{charData}";
    }

    private async Task<string> HandleUpdateCharacterAsync(string[] parts)
    {
        if (parts.Length < 3)
            return "ERROR|Invalid UPDATE_CHARACTER format. Expected: UPDATE_CHARACTER|username|characterData";

        var username = parts[1];
        var characterData = parts[2];

        var dataParts = characterData.Split('|');
        if (dataParts.Length < 8)
            return "ERROR|Invalid character data";

        try
        {
            var character = new Character
            {
                Owner = username,
                Name = dataParts[0],
                Level = int.Parse(dataParts[1]),
                Experience = int.Parse(dataParts[2]),
                Health = float.Parse(dataParts[3]),
                MaxHealth = float.Parse(dataParts[4]),
                PositionX = float.Parse(dataParts[5]),
                PositionY = float.Parse(dataParts[6]),
                PositionZ = float.Parse(dataParts[7])
            };

            var result = await _characterService.UpdateCharacterAsync(character);
            return result.Success ? $"SUCCESS|{result.Message}" : $"ERROR|{result.Message}";
        }
        catch (Exception ex)
        {
            return $"ERROR|Failed to update character: {ex.Message}";
        }
    }
}
