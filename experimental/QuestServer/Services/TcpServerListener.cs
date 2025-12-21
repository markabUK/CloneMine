using System.Net;
using System.Net.Sockets;
using CloneMine.QuestServer.Interfaces;
using CloneMine.QuestServer.Models;

namespace CloneMine.QuestServer.Services;

public class TcpServerListener
{
    private readonly ServerConfiguration _config;
    private readonly IClientHandler _clientHandler;
    private TcpListener? _listener;
    private bool _running;
    private readonly CancellationTokenSource _cancellationTokenSource;

    public bool IsRunning => _running;

    public TcpServerListener(ServerConfiguration config, IClientHandler clientHandler)
    {
        _config = config ?? throw new ArgumentNullException(nameof(config));
        _clientHandler = clientHandler ?? throw new ArgumentNullException(nameof(clientHandler));
        _cancellationTokenSource = new CancellationTokenSource();
    }

    public async Task StartAsync(int port)
    {
        if (_running) throw new InvalidOperationException("Server is already running");

        try
        {
            _listener = new TcpListener(IPAddress.Any, port);
            _listener.Start();
            _running = true;

            Console.WriteLine($"[QuestServer] Started on port {port}");
            Console.WriteLine("[QuestServer] Press Ctrl+C to stop");
            Console.WriteLine();

            await AcceptClientsAsync(_cancellationTokenSource.Token);
        }
        catch (Exception ex)
        {
            Console.WriteLine($"[QuestServer] Failed to start: {ex.Message}");
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

                _ = Task.Run(async () =>
                {
                    try
                    {
                        await _clientHandler.HandleClientAsync(client);
                    }
                    catch (Exception ex)
                    {
                        Console.WriteLine($"[QuestServer] Unhandled client error: {ex.Message}");
                    }
                }, cancellationToken);
            }
            catch (ObjectDisposedException)
            {
                break;
            }
            catch (Exception ex) when (_running)
            {
                Console.WriteLine($"[QuestServer] Error accepting client: {ex.Message}");
                await Task.Delay(100, cancellationToken);
            }
        }
    }

    public Task StopAsync()
    {
        if (!_running) return Task.CompletedTask;

        Console.WriteLine("[QuestServer] Shutting down...");
        _running = false;
        _cancellationTokenSource.Cancel();
        _listener?.Stop();
        Console.WriteLine("[QuestServer] Shutdown complete");

        return Task.CompletedTask;
    }
}
