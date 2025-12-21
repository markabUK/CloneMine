namespace CloneMine.LoginServer.Interfaces;

/// <summary>
/// Interface for message encryption/decryption
/// Single Responsibility: Message security
/// </summary>
public interface IEncryptionService
{
    byte[] Encrypt(byte[] data);
    byte[] Decrypt(byte[] data);
    string EncryptString(string text);
    string DecryptString(string encryptedText);
}
