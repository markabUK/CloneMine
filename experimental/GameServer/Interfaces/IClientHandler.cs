using System.Net.Sockets;

namespace CloneMine.GameServer.Interfaces;

public interface IClientHandler
{
    Task HandleClientAsync(TcpClient client);
}
