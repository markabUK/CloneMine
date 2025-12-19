using CloneMine.CharacterServer.Models;

namespace CloneMine.CharacterServer.Interfaces;

/// <summary>
/// Interface for character data persistence
/// </summary>
public interface ICharacterRepository
{
    Task<IEnumerable<Character>> GetCharactersAsync(string username);
    Task<Character?> GetCharacterAsync(string username, string characterName);
    Task<bool> CreateCharacterAsync(Character character);
    Task<bool> UpdateCharacterAsync(Character character);
    Task<bool> DeleteCharacterAsync(string username, string characterName);
    Task<int> GetCharacterCountAsync(string username);
}
