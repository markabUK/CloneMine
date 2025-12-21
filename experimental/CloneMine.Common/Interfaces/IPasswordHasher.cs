namespace CloneMine.Common.Interfaces;

/// <summary>
/// Interface for password hashing services
/// Interface Segregation Principle: Focused on password hashing only
/// </summary>
public interface IPasswordHasher
{
    /// <summary>
    /// Hashes a password using PBKDF2-SHA256 with 100,000 iterations
    /// </summary>
    /// <param name="password">Plain text password</param>
    /// <returns>Base64-encoded salt and hash</returns>
    string HashPassword(string password);

    /// <summary>
    /// Verifies a password against a hash using constant-time comparison
    /// </summary>
    /// <param name="password">Plain text password to verify</param>
    /// <param name="hash">Base64-encoded salt and hash to verify against</param>
    /// <returns>True if password matches, false otherwise</returns>
    bool VerifyPassword(string password, string hash);
}
