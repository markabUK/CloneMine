using System.Net.Sockets;

namespace CloneMine.LoginServer.Interfaces;

/// <summary>
/// Interface for client connection handling
/// Single Responsibility: Client connection lifecycle
/// </summary>
public interface IClientHandler
{
    Task HandleClientAsync(TcpClient client);
}
