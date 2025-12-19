namespace CloneMine.ChatServer.Interfaces;

public interface IEncryptionService
{
    byte[] Encrypt(byte[] data);
    byte[] Decrypt(byte[] data);
}
