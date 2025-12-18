using CloneMine.LoginServer.Models;

namespace CloneMine.LoginServer.Interfaces;

/// <summary>
/// Interface for authentication operations
/// Single Responsibility: Authentication logic
/// </summary>
public interface IAuthenticationService
{
    Task<LoginResult> LoginAsync(string username, string password);
    Task<LoginResult> RegisterAsync(string username, string password);
    Task<int> GetCharacterCountAsync(string username);
}
