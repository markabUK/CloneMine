using CloneMine.CharacterServer.Models;

namespace CloneMine.CharacterServer.Interfaces;

/// <summary>
/// Interface for character business logic
/// </summary>
public interface ICharacterService
{
    Task<IEnumerable<Character>> ListCharactersAsync(string username);
    Task<(bool Success, string Message)> CreateCharacterAsync(string username, string characterName);
    Task<(bool Success, string Message)> DeleteCharacterAsync(string username, string characterName);
    Task<Character?> GetCharacterAsync(string username, string characterName);
    Task<(bool Success, string Message)> UpdateCharacterAsync(Character character);
}
