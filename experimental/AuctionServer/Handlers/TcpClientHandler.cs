using System.Net.Sockets;
using System.Text;
using CloneMine.AuctionServer.Interfaces;
using CloneMine.Common.Interfaces;

namespace CloneMine.AuctionServer.Handlers;

/// <summary>
/// Client connection handler
/// Single Responsibility: Managing individual client connections
/// </summary>
public class TcpClientHandler : IClientHandler
{
    private readonly IMessageHandler _messageHandler;
    private readonly IEncryptionService _encryptionService;
    private readonly IRateLimiter _rateLimiter;

    public TcpClientHandler(
        IMessageHandler messageHandler,
        IEncryptionService encryptionService,
        IRateLimiter rateLimiter)
    {
        _messageHandler = messageHandler ?? throw new ArgumentNullException(nameof(messageHandler));
        _encryptionService = encryptionService ?? throw new ArgumentNullException(nameof(encryptionService));
        _rateLimiter = rateLimiter ?? throw new ArgumentNullException(nameof(rateLimiter));
    }

    public async Task HandleClientAsync(TcpClient client)
    {
        if (client == null)
        {
            throw new ArgumentNullException(nameof(client));
        }

        var remoteEndPoint = client.Client.RemoteEndPoint?.ToString() ?? "Unknown";
        var clientIp = client.Client.RemoteEndPoint?.ToString()?.Split(':')[0] ?? "Unknown";
        
        // Check rate limit
        if (!_rateLimiter.AllowRequest(clientIp))
        {
            Console.WriteLine($"[AuctionServer] Rate limit exceeded for IP: {clientIp} - connection rejected");
            client.Close();
            return;
        }
        
        Console.WriteLine($"[AuctionServer] Client connected: {remoteEndPoint}");

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
                        // Try to decrypt
                        var encryptedData = new byte[bytesRead];
                        Array.Copy(buffer, 0, encryptedData, 0, bytesRead);
                        var decryptedData = _encryptionService.Decrypt(encryptedData);
                        message = Encoding.UTF8.GetString(decryptedData).Trim();
                    }
                    catch
                    {
                        // Fallback to plain text
                        message = Encoding.UTF8.GetString(buffer, 0, bytesRead).Trim();
                    }

                    if (string.IsNullOrWhiteSpace(message))
                    {
                        continue;
                    }

                    // Process message
                    var response = await _messageHandler.HandleMessageAsync(message);

                    if (string.IsNullOrEmpty(response))
                    {
                        response = "ERROR|Internal server error";
                    }

                    // Send response
                    try
                    {
                        var responseBytes = Encoding.UTF8.GetBytes(response + "\n");
                        var encryptedResponse = _encryptionService.Encrypt(responseBytes);
                        await stream.WriteAsync(encryptedResponse, 0, encryptedResponse.Length);
                    }
                    catch
                    {
                        // Fallback to plain text
                        var responseBytes = Encoding.UTF8.GetBytes(response + "\n");
                        await stream.WriteAsync(responseBytes, 0, responseBytes.Length);
                    }
                }
                catch (Exception ex)
                {
                    Console.WriteLine($"[AuctionServer] Error processing message: {ex.Message}");
                    var errorResponse = Encoding.UTF8.GetBytes("ERROR|Message processing failed\n");
                    await stream.WriteAsync(errorResponse, 0, errorResponse.Length);
                }
            }
        }
        catch (Exception ex)
        {
            Console.WriteLine($"[AuctionServer] Client error ({remoteEndPoint}): {ex.Message}");
        }
        finally
        {
            client.Close();
            Console.WriteLine($"[AuctionServer] Client disconnected: {remoteEndPoint}");
        }
    }
}
