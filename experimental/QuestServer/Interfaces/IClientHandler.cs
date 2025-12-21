using System.Net.Sockets;

namespace CloneMine.QuestServer.Interfaces;

public interface IClientHandler
{
    Task HandleClientAsync(TcpClient client);
}
