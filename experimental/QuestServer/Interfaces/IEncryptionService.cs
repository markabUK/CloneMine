namespace CloneMine.QuestServer.Interfaces;

public interface IEncryptionService
{
    byte[] Encrypt(byte[] data);
    byte[] Decrypt(byte[] data);
}
