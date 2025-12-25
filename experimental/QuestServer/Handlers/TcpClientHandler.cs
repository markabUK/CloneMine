using System.Net.Sockets;
using System.Text;
using CloneMine.QuestServer.Interfaces;

namespace CloneMine.QuestServer.Handlers;

public class TcpClientHandler : IClientHandler
{
    private readonly IMessageHandler _messageHandler;
    private readonly CloneMine.QuestServer.Interfaces.IEncryptionService _encryptionService;
    private readonly CloneMine.Common.Interfaces.IRateLimiter _rateLimiter;

    public TcpClientHandler(IMessageHandler messageHandler, CloneMine.QuestServer.Interfaces.IEncryptionService encryptionService, CloneMine.Common.Interfaces.IRateLimiter rateLimiter)
    {
        _messageHandler = messageHandler ?? throw new ArgumentNullException(nameof(messageHandler));
        _encryptionService = encryptionService ?? throw new ArgumentNullException(nameof(encryptionService));
        _rateLimiter = rateLimiter ?? throw new ArgumentNullException(nameof(rateLimiter));
    }

    public async Task HandleClientAsync(TcpClient client)
    {
        if (client == null) throw new ArgumentNullException(nameof(client));

        var remoteEndPoint = client.Client.RemoteEndPoint?.ToString() ?? "Unknown";
        var clientIp = client.Client.RemoteEndPoint?.ToString()?.Split(':')[0] ?? "Unknown";
        
        // Check rate limit
        if (!_rateLimiter.AllowRequest(clientIp))
        {
            Console.WriteLine($"[QuestServer] Rate limit exceeded for IP: {clientIp} - connection rejected");
            client.Close();
            return;
        }
        
        Console.WriteLine($"[QuestServer] Client connected: {remoteEndPoint}");

        try
        {
            using var stream = client.GetStream();
            var buffer = new byte[4096];

            while (client.Connected)
            {
                var bytesRead = await stream.ReadAsync(buffer, 0, buffer.Length);
                if (bytesRead == 0) break;

                try
                {
                    string message;
                    try
                    {
                        var encryptedData = new byte[bytesRead];
                        Array.Copy(buffer, 0, encryptedData, 0, bytesRead);
                        var decryptedData = _encryptionService.Decrypt(encryptedData);
                        message = Encoding.UTF8.GetString(decryptedData).Trim();
                    }
                    catch
                    {
                        message = Encoding.UTF8.GetString(buffer, 0, bytesRead).Trim();
                    }

                    if (string.IsNullOrWhiteSpace(message)) continue;

                    var response = await _messageHandler.HandleMessageAsync(message);
                    if (string.IsNullOrEmpty(response)) response = "ERROR|Internal server error";

                    try
                    {
                        var responseBytes = Encoding.UTF8.GetBytes(response + "\n");
                        var encryptedResponse = _encryptionService.Encrypt(responseBytes);
                        await stream.WriteAsync(encryptedResponse, 0, encryptedResponse.Length);
                    }
                    catch
                    {
                        var responseBytes = Encoding.UTF8.GetBytes(response + "\n");
                        await stream.WriteAsync(responseBytes, 0, responseBytes.Length);
                    }
                }
                catch (Exception ex)
                {
                    Console.WriteLine($"[QuestServer] Error processing message: {ex.Message}");
                }
            }
        }
        catch (Exception ex)
        {
            Console.WriteLine($"[QuestServer] Client error ({remoteEndPoint}): {ex.Message}");
        }
        finally
        {
            client.Close();
            Console.WriteLine($"[QuestServer] Client disconnected: {remoteEndPoint}");
        }
    }
}
