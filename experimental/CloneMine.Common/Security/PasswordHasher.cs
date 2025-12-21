using System.Security.Cryptography;
using System.Text;
using CloneMine.Common.Interfaces;

namespace CloneMine.Common.Security;

/// <summary>
/// Password hasher using PBKDF2-SHA256 with 100,000 iterations
/// Single Responsibility: Secure password hashing
/// Open/Closed: Implements IPasswordHasher interface for extensibility
/// </summary>
public class PasswordHasher : IPasswordHasher
{
    private const int SaltSize = 16;
    private const int HashSize = 32;
    private const int Iterations = 100000;

    public string HashPassword(string password)
    {
        if (string.IsNullOrEmpty(password))
        {
            throw new ArgumentNullException(nameof(password), "Password cannot be null or empty");
        }

        // Generate salt
        byte[] salt = RandomNumberGenerator.GetBytes(SaltSize);

        // Generate hash using PBKDF2
        byte[] hash = Rfc2898DeriveBytes.Pbkdf2(
            password,
            salt,
            Iterations,
            HashAlgorithmName.SHA256,
            HashSize);

        // Combine salt and hash
        byte[] hashBytes = new byte[SaltSize + HashSize];
        Array.Copy(salt, 0, hashBytes, 0, SaltSize);
        Array.Copy(hash, 0, hashBytes, SaltSize, HashSize);

        return Convert.ToBase64String(hashBytes);
    }

    public bool VerifyPassword(string password, string hash)
    {
        if (string.IsNullOrEmpty(password))
        {
            return false;
        }

        if (string.IsNullOrEmpty(hash))
        {
            return false;
        }

        try
        {
            // Extract salt and hash from stored hash
            byte[] hashBytes = Convert.FromBase64String(hash);

            if (hashBytes.Length != SaltSize + HashSize)
            {
                return false;
            }

            byte[] salt = new byte[SaltSize];
            Array.Copy(hashBytes, 0, salt, 0, SaltSize);

            byte[] storedHash = new byte[HashSize];
            Array.Copy(hashBytes, SaltSize, storedHash, 0, HashSize);

            // Compute hash of provided password
            byte[] testHash = Rfc2898DeriveBytes.Pbkdf2(
                password,
                salt,
                Iterations,
                HashAlgorithmName.SHA256,
                HashSize);

            // Use constant-time comparison to prevent timing attacks
            return CryptographicOperations.FixedTimeEquals(testHash, storedHash);
        }
        catch
        {
            return false;
        }
    }
}
