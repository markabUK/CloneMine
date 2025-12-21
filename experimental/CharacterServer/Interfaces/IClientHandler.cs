using System.Net.Sockets;

namespace CloneMine.CharacterServer.Interfaces;

public interface IClientHandler
{
    Task HandleClientAsync(TcpClient client);
}
