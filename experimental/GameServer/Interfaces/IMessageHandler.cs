namespace CloneMine.GameServer.Interfaces;

public interface IMessageHandler
{
    Task<string> HandleMessageAsync(string message);
}
