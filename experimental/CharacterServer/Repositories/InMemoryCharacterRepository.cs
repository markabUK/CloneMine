using System.Collections.Concurrent;
using CloneMine.CharacterServer.Interfaces;
using CloneMine.CharacterServer.Models;

namespace CloneMine.CharacterServer.Repositories;

public class InMemoryCharacterRepository : ICharacterRepository
{
    private readonly ConcurrentDictionary<string, List<Character>> _accountCharacters;

    public InMemoryCharacterRepository()
    {
        _accountCharacters = new ConcurrentDictionary<string, List<Character>>(StringComparer.OrdinalIgnoreCase);
    }

    public Task<IEnumerable<Character>> GetCharactersAsync(string username)
    {
        if (string.IsNullOrWhiteSpace(username))
            return Task.FromResult<IEnumerable<Character>>(Array.Empty<Character>());

        if (!_accountCharacters.TryGetValue(username, out var characters))
            return Task.FromResult<IEnumerable<Character>>(Array.Empty<Character>());

        return Task.FromResult<IEnumerable<Character>>(characters.ToList());
    }

    public Task<Character?> GetCharacterAsync(string username, string characterName)
    {
        if (string.IsNullOrWhiteSpace(username) || string.IsNullOrWhiteSpace(characterName))
            return Task.FromResult<Character?>(null);

        if (!_accountCharacters.TryGetValue(username, out var characters))
            return Task.FromResult<Character?>(null);

        var character = characters.FirstOrDefault(c => 
            c.Name.Equals(characterName, StringComparison.OrdinalIgnoreCase));
        return Task.FromResult(character);
    }

    public Task<bool> CreateCharacterAsync(Character character)
    {
        if (character == null) throw new ArgumentNullException(nameof(character));
        if (string.IsNullOrWhiteSpace(character.Owner)) return Task.FromResult(false);

        var characters = _accountCharacters.GetOrAdd(character.Owner, new List<Character>());
        
        lock (characters)
        {
            if (characters.Any(c => c.Name.Equals(character.Name, StringComparison.OrdinalIgnoreCase)))
                return Task.FromResult(false);
            
            characters.Add(character);
        }

        return Task.FromResult(true);
    }

    public Task<bool> UpdateCharacterAsync(Character character)
    {
        if (character == null) throw new ArgumentNullException(nameof(character));
        if (!_accountCharacters.TryGetValue(character.Owner, out var characters)) 
            return Task.FromResult(false);

        lock (characters)
        {
            var existing = characters.FirstOrDefault(c => 
                c.Name.Equals(character.Name, StringComparison.OrdinalIgnoreCase));
            if (existing == null) return Task.FromResult(false);

            existing.Level = character.Level;
            existing.Experience = character.Experience;
            existing.Health = character.Health;
            existing.MaxHealth = character.MaxHealth;
            existing.PositionX = character.PositionX;
            existing.PositionY = character.PositionY;
            existing.PositionZ = character.PositionZ;
        }

        return Task.FromResult(true);
    }

    public Task<bool> DeleteCharacterAsync(string username, string characterName)
    {
        if (!_accountCharacters.TryGetValue(username, out var characters)) 
            return Task.FromResult(false);

        lock (characters)
        {
            var character = characters.FirstOrDefault(c => 
                c.Name.Equals(characterName, StringComparison.OrdinalIgnoreCase));
            if (character == null) return Task.FromResult(false);

            characters.Remove(character);
        }

        return Task.FromResult(true);
    }

    public Task<int> GetCharacterCountAsync(string username)
    {
        if (!_accountCharacters.TryGetValue(username, out var characters))
            return Task.FromResult(0);

        return Task.FromResult(characters.Count);
    }
}
