namespace CloneMine.Common.Interfaces;

/// <summary>
/// Interface for encryption/decryption services
/// Interface Segregation Principle: Focused on encryption operations only
/// </summary>
public interface IEncryptionService
{
    /// <summary>
    /// Encrypts data using AES-256-CBC
    /// </summary>
    /// <param name="data">Data to encrypt</param>
    /// <returns>Encrypted data</returns>
    byte[] Encrypt(byte[] data);

    /// <summary>
    /// Decrypts data using AES-256-CBC
    /// </summary>
    /// <param name="data">Encrypted data</param>
    /// <returns>Decrypted data</returns>
    byte[] Decrypt(byte[] data);

    /// <summary>
    /// Encrypts a string message
    /// </summary>
    /// <param name="message">Plain text message</param>
    /// <returns>Encrypted message as base64 string</returns>
    string EncryptString(string message);

    /// <summary>
    /// Decrypts a string message
    /// </summary>
    /// <param name="encryptedMessage">Encrypted message as base64 string</param>
    /// <returns>Decrypted plain text message</returns>
    string DecryptString(string encryptedMessage);
}
