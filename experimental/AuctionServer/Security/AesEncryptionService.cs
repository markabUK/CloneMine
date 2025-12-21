using System.Security.Cryptography;
using System.Text;
using CloneMine.AuctionServer.Interfaces;

namespace CloneMine.AuctionServer.Security;

/// <summary>
/// AES encryption service for secure message transmission
/// Single Responsibility: Message encryption/decryption
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
            throw new InvalidOperationException("Failed to decrypt data. Invalid key or corrupted data.");
        }
    }

    public string EncryptString(string text)
    {
        if (string.IsNullOrEmpty(text))
        {
            throw new ArgumentNullException(nameof(text), "Text cannot be null or empty");
        }

        byte[] encrypted = Encrypt(Encoding.UTF8.GetBytes(text));
        return Convert.ToBase64String(encrypted);
    }

    public string DecryptString(string encryptedText)
    {
        if (string.IsNullOrEmpty(encryptedText))
        {
            throw new ArgumentNullException(nameof(encryptedText), "Encrypted text cannot be null or empty");
        }

        try
        {
            byte[] data = Convert.FromBase64String(encryptedText);
            byte[] decrypted = Decrypt(data);
            return Encoding.UTF8.GetString(decrypted);
        }
        catch (FormatException)
        {
            throw new InvalidOperationException("Invalid encrypted text format");
        }
    }
}
