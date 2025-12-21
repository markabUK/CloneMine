using System.Net.Sockets;

namespace CloneMine.ChatServer.Interfaces;

public interface IClientHandler
{
    Task HandleClientAsync(TcpClient client);
}
