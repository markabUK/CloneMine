namespace CloneMine.LoginServer.Interfaces;

/// <summary>
/// Interface for password hashing operations
/// Single Responsibility: Password security
/// </summary>
public interface IPasswordHasher
{
    string HashPassword(string password);
    bool VerifyPassword(string password, string hash);
}
