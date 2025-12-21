using CloneMine.CharacterServer.Interfaces;
using CloneMine.CharacterServer.Models;

namespace CloneMine.CharacterServer.Services;

public class CharacterService : ICharacterService
{
    private readonly ICharacterRepository _repository;
    private readonly IInputValidator _validator;
    private readonly ServerConfiguration _config;

    public CharacterService(
        ICharacterRepository repository,
        IInputValidator validator,
        ServerConfiguration config)
    {
        _repository = repository ?? throw new ArgumentNullException(nameof(repository));
        _validator = validator ?? throw new ArgumentNullException(nameof(validator));
        _config = config ?? throw new ArgumentNullException(nameof(config));
    }

    public Task<IEnumerable<Character>> ListCharactersAsync(string username)
    {
        var validation = _validator.ValidateUsername(username);
        if (!validation.IsValid)
            return Task.FromResult<IEnumerable<Character>>(Array.Empty<Character>());

        return _repository.GetCharactersAsync(username);
    }

    public async Task<(bool Success, string Message)> CreateCharacterAsync(string username, string characterName)
    {
        var usernameValidation = _validator.ValidateUsername(username);
        if (!usernameValidation.IsValid)
            return (false, usernameValidation.ErrorMessage);

        var nameValidation = _validator.ValidateCharacterName(characterName);
        if (!nameValidation.IsValid)
            return (false, nameValidation.ErrorMessage);

        var count = await _repository.GetCharacterCountAsync(username);
        if (count >= _config.MaxCharactersPerAccount)
            return (false, $"Maximum {_config.MaxCharactersPerAccount} characters per account");

        var character = new Character
        {
            Name = characterName,
            Owner = username,
            Level = 1,
            Experience = 0,
            Health = 100.0f,
            MaxHealth = 100.0f,
            PositionX = 0.0f,
            PositionY = 64.0f,
            PositionZ = 0.0f
        };

        var created = await _repository.CreateCharacterAsync(character);
        if (!created)
            return (false, "Character name already exists");

        Console.WriteLine($"[CharacterServer] Character created: {username}/{characterName}");
        return (true, $"Character created: {characterName}");
    }

    public async Task<(bool Success, string Message)> DeleteCharacterAsync(string username, string characterName)
    {
        var usernameValidation = _validator.ValidateUsername(username);
        if (!usernameValidation.IsValid)
            return (false, usernameValidation.ErrorMessage);

        var nameValidation = _validator.ValidateCharacterName(characterName);
        if (!nameValidation.IsValid)
            return (false, nameValidation.ErrorMessage);

        var deleted = await _repository.DeleteCharacterAsync(username, characterName);
        if (!deleted)
            return (false, "Character not found");

        Console.WriteLine($"[CharacterServer] Character deleted: {username}/{characterName}");
        return (true, $"Character deleted: {characterName}");
    }

    public Task<Character?> GetCharacterAsync(string username, string characterName)
    {
        return _repository.GetCharacterAsync(username, characterName);
    }

    public async Task<(bool Success, string Message)> UpdateCharacterAsync(Character character)
    {
        if (character == null)
            return (false, "Character cannot be null");

        var updated = await _repository.UpdateCharacterAsync(character);
        if (!updated)
            return (false, "Character not found");

        Console.WriteLine($"[CharacterServer] Character updated: {character.Owner}/{character.Name}");
        return (true, $"Character updated: {character.Name}");
    }
}
