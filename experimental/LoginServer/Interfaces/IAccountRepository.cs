using CloneMine.LoginServer.Models;

namespace CloneMine.LoginServer.Interfaces;

/// <summary>
/// Interface for account data persistence
/// Single Responsibility: Account data storage
/// </summary>
public interface IAccountRepository
{
    Task<Account?> GetAccountAsync(string username);
    Task<bool> CreateAccountAsync(Account account);
    Task<bool> UpdateAccountAsync(Account account);
    Task<bool> DeleteAccountAsync(string username);
    Task<bool> AccountExistsAsync(string username);
}
