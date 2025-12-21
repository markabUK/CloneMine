using System.Collections.Concurrent;
using CloneMine.LoginServer.Interfaces;
using CloneMine.LoginServer.Models;

namespace CloneMine.LoginServer.Repositories;

/// <summary>
/// In-memory account repository
/// Single Responsibility: Account data storage
/// Open/Closed Principle: Can be extended with database implementation
/// </summary>
public class InMemoryAccountRepository : IAccountRepository
{
    private readonly ConcurrentDictionary<string, Account> _accounts;

    public InMemoryAccountRepository()
    {
        _accounts = new ConcurrentDictionary<string, Account>(StringComparer.OrdinalIgnoreCase);
        InitializeTestAccounts();
    }

    private void InitializeTestAccounts()
    {
        // Note: These use plain text for demonstration
        // In production, passwords should be pre-hashed
        var testAccounts = new[]
        {
            new Account
            {
                Username = "test",
                PasswordHash = "test123", // Will be hashed properly by AuthenticationService
                CharacterCount = 2,
                CreatedAt = DateTime.UtcNow.AddDays(-30),
                LastLogin = DateTime.UtcNow.AddHours(-2)
            },
            new Account
            {
                Username = "admin",
                PasswordHash = "admin123",
                CharacterCount = 1,
                CreatedAt = DateTime.UtcNow.AddDays(-60),
                LastLogin = DateTime.UtcNow.AddHours(-5)
            },
            new Account
            {
                Username = "player1",
                PasswordHash = "password1",
                CharacterCount = 0,
                CreatedAt = DateTime.UtcNow.AddDays(-10),
                LastLogin = DateTime.UtcNow.AddDays(-1)
            }
        };

        foreach (var account in testAccounts)
        {
            _accounts.TryAdd(account.Username, account);
        }
    }

    public Task<Account?> GetAccountAsync(string username)
    {
        if (string.IsNullOrWhiteSpace(username))
        {
            return Task.FromResult<Account?>(null);
        }

        _accounts.TryGetValue(username, out var account);
        return Task.FromResult(account);
    }

    public Task<bool> CreateAccountAsync(Account account)
    {
        if (account == null)
        {
            throw new ArgumentNullException(nameof(account));
        }

        if (string.IsNullOrWhiteSpace(account.Username))
        {
            return Task.FromResult(false);
        }

        return Task.FromResult(_accounts.TryAdd(account.Username, account));
    }

    public Task<bool> UpdateAccountAsync(Account account)
    {
        if (account == null)
        {
            throw new ArgumentNullException(nameof(account));
        }

        if (string.IsNullOrWhiteSpace(account.Username))
        {
            return Task.FromResult(false);
        }

        _accounts[account.Username] = account;
        return Task.FromResult(true);
    }

    public Task<bool> DeleteAccountAsync(string username)
    {
        if (string.IsNullOrWhiteSpace(username))
        {
            return Task.FromResult(false);
        }

        return Task.FromResult(_accounts.TryRemove(username, out _));
    }

    public Task<bool> AccountExistsAsync(string username)
    {
        if (string.IsNullOrWhiteSpace(username))
        {
            return Task.FromResult(false);
        }

        return Task.FromResult(_accounts.ContainsKey(username));
    }
}
