using System.Net;
using System.Net.Sockets;
using System.Collections.Concurrent;
using System.Text;
using CloneMine.ChatServer.Interfaces;
using CloneMine.ChatServer.Models;
using CloneMine.ChatServer.Handlers;

namespace CloneMine.ChatServer.Services;

public class TcpServerListener
{
    private readonly ServerConfiguration _config;
    private readonly IClientHandler _clientHandler;
    private readonly ChatMessageHandler _messageHandler;
    private readonly IEncryptionService _encryptionService;
    private TcpListener? _listener;
    private bool _running;
    private readonly CancellationTokenSource _cancellationTokenSource;
    private readonly ConcurrentBag<TcpClient> _clients;

    public bool IsRunning => _running;

    public TcpServerListener(
        ServerConfiguration config,
        IClientHandler clientHandler,
        ChatMessageHandler messageHandler,
        IEncryptionService encryptionService)
    {
        _config = config ?? throw new ArgumentNullException(nameof(config));
        _clientHandler = clientHandler ?? throw new ArgumentNullException(nameof(clientHandler));
        _messageHandler = messageHandler ?? throw new ArgumentNullException(nameof(messageHandler));
        _encryptionService = encryptionService ?? throw new ArgumentNullException(nameof(encryptionService));
        _cancellationTokenSource = new CancellationTokenSource();
        _clients = new ConcurrentBag<TcpClient>();

        _messageHandler.SetBroadcastAction(BroadcastMessage);
    }

    public async Task StartAsync(int port)
    {
        if (_running) throw new InvalidOperationException("Server is already running");

        try
        {
            _listener = new TcpListener(IPAddress.Any, port);
            _listener.Start();
            _running = true;

            Console.WriteLine($"[ChatServer] Started on port {port}");
            Console.WriteLine("[ChatServer] Press Ctrl+C to stop");
            Console.WriteLine();

            await AcceptClientsAsync(_cancellationTokenSource.Token);
        }
        catch (Exception ex)
        {
            Console.WriteLine($"[ChatServer] Failed to start: {ex.Message}");
            throw;
        }
    }

    private async Task AcceptClientsAsync(CancellationToken cancellationToken)
    {
        while (_running && !cancellationToken.IsCancellationRequested && _listener != null)
        {
            try
            {
                var client = await _listener.AcceptTcpClientAsync();
                _clients.Add(client);

                _ = Task.Run(async () =>
                {
                    try
                    {
                        await _clientHandler.HandleClientAsync(client);
                    }
                    catch (Exception ex)
                    {
                        Console.WriteLine($"[ChatServer] Unhandled client error: {ex.Message}");
                    }
                }, cancellationToken);
            }
            catch (ObjectDisposedException)
            {
                break;
            }
            catch (Exception ex) when (_running)
            {
                Console.WriteLine($"[ChatServer] Error accepting client: {ex.Message}");
                await Task.Delay(100, cancellationToken);
            }
        }
    }

    private void BroadcastMessage(string message)
    {
        var messageBytes = Encoding.UTF8.GetBytes(message + "\n");

        foreach (var client in _clients)
        {
            try
            {
                if (client.Connected)
                {
                    var stream = client.GetStream();
                    try
                    {
                        var encrypted = _encryptionService.Encrypt(messageBytes);
                        stream.Write(encrypted, 0, encrypted.Length);
                    }
                    catch
                    {
                        stream.Write(messageBytes, 0, messageBytes.Length);
                    }
                }
            }
            catch { /* Client disconnected */ }
        }
    }

    public Task StopAsync()
    {
        if (!_running) return Task.CompletedTask;

        Console.WriteLine("[ChatServer] Shutting down...");
        _running = false;
        _cancellationTokenSource.Cancel();
        _listener?.Stop();

        foreach (var client in _clients)
        {
            client.Close();
        }

        Console.WriteLine("[ChatServer] Shutdown complete");
        return Task.CompletedTask;
    }
}
