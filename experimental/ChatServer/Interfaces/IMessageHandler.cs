namespace CloneMine.ChatServer.Interfaces;

public interface IMessageHandler
{
    Task<string> HandleMessageAsync(string message);
}
