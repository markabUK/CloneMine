namespace CloneMine.CharacterServer.Interfaces;

public interface IMessageHandler
{
    Task<string> HandleMessageAsync(string message);
}
