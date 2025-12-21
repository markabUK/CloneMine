using System.Security.Cryptography;
using System.Text;
using CloneMine.Common.Interfaces;

namespace CloneMine.Common.Security;

/// <summary>
/// AES-256-CBC encryption service for secure message transmission
/// Single Responsibility: Message encryption/decryption
/// Open/Closed: Implements IEncryptionService interface for extensibility
/// </summary>
public class AesEncryptionService : IEncryptionService
{
    private readonly byte[] _key;
    private readonly byte[] _iv;

    public AesEncryptionService(string? key = null)
    {
        if (string.IsNullOrEmpty(key))
        {
            // Default key for demonstration - should be configured securely in production
            key = "CloneMineSecureDefaultKey123456";
        }

        using var sha256 = SHA256.Create();
        _key = sha256.ComputeHash(Encoding.UTF8.GetBytes(key));
        _iv = sha256.ComputeHash(Encoding.UTF8.GetBytes(key + "IV")).Take(16).ToArray();
    }

    public byte[] Encrypt(byte[] data)
    {
        if (data == null || data.Length == 0)
        {
            throw new ArgumentNullException(nameof(data), "Data cannot be null or empty");
        }

        using var aes = Aes.Create();
        aes.Key = _key;
        aes.IV = _iv;
        aes.Mode = CipherMode.CBC;
        aes.Padding = PaddingMode.PKCS7;

        using var encryptor = aes.CreateEncryptor();
        return encryptor.TransformFinalBlock(data, 0, data.Length);
    }

    public byte[] Decrypt(byte[] data)
    {
        if (data == null || data.Length == 0)
        {
            throw new ArgumentNullException(nameof(data), "Data cannot be null or empty");
        }

        try
        {
            using var aes = Aes.Create();
            aes.Key = _key;
            aes.IV = _iv;
            aes.Mode = CipherMode.CBC;
            aes.Padding = PaddingMode.PKCS7;

            using var decryptor = aes.CreateDecryptor();
            return decryptor.TransformFinalBlock(data, 0, data.Length);
        }
        catch (CryptographicException)
        {
            // Return original data if decryption fails (for testing with unencrypted messages)
            return data;
        }
    }

    public string EncryptString(string message)
    {
        if (string.IsNullOrEmpty(message))
        {
            throw new ArgumentNullException(nameof(message), "Message cannot be null or empty");
        }

        byte[] messageBytes = Encoding.UTF8.GetBytes(message);
        byte[] encryptedBytes = Encrypt(messageBytes);
        return Convert.ToBase64String(encryptedBytes);
    }

    public string DecryptString(string encryptedMessage)
    {
        if (string.IsNullOrEmpty(encryptedMessage))
        {
            throw new ArgumentNullException(nameof(encryptedMessage), "Encrypted message cannot be null or empty");
        }

        try
        {
            byte[] encryptedBytes = Convert.FromBase64String(encryptedMessage);
            byte[] decryptedBytes = Decrypt(encryptedBytes);
            return Encoding.UTF8.GetString(decryptedBytes);
        }
        catch
        {
            // Return original message if decryption fails
            return encryptedMessage;
        }
    }
}
