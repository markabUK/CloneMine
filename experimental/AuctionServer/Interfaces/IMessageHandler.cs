namespace CloneMine.AuctionServer.Interfaces;

/// <summary>
/// Interface for handling client messages
/// Single Responsibility: Message processing
/// </summary>
public interface IMessageHandler
{
    Task<string> HandleMessageAsync(string message);
}
