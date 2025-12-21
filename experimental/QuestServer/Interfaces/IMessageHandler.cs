namespace CloneMine.QuestServer.Interfaces;

public interface IMessageHandler
{
    Task<string> HandleMessageAsync(string message);
}
