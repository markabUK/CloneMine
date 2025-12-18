using System.Net;
using System.Net.Sockets;
using CloneMine.LoginServer.Interfaces;
using CloneMine.LoginServer.Models;

namespace CloneMine.LoginServer.Services;

/// <summary>
/// TCP server listener
/// Single Responsibility: Managing server lifecycle and accepting connections
/// </summary>
public class TcpServerListener
{
    private readonly ServerConfiguration _config;
    private readonly IClientHandler _clientHandler;
    private TcpListener? _listener;
    private bool _running;
    private readonly CancellationTokenSource _cancellationTokenSource;

    public bool IsRunning => _running;

    public TcpServerListener(
        ServerConfiguration config,
        IClientHandler clientHandler)
    {
        _config = config ?? throw new ArgumentNullException(nameof(config));
        _clientHandler = clientHandler ?? throw new ArgumentNullException(nameof(clientHandler));
        _cancellationTokenSource = new CancellationTokenSource();
    }

    public async Task StartAsync(int port)
    {
        if (_running)
        {
            throw new InvalidOperationException("Server is already running");
        }

        try
        {
            _listener = new TcpListener(IPAddress.Any, port);
            _listener.Start();
            _running = true;

            Console.WriteLine($"[LoginServer] Started on port {port}");
            Console.WriteLine("[LoginServer] Press Ctrl+C to stop");
            Console.WriteLine();

            // Accept clients in background
            await AcceptClientsAsync(_cancellationTokenSource.Token);
        }
        catch (Exception ex)
        {
            Console.WriteLine($"[LoginServer] Failed to start: {ex.Message}");
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

                // Handle client in separate task
                _ = Task.Run(async () =>
                {
                    try
                    {
                        await _clientHandler.HandleClientAsync(client);
                    }
                    catch (Exception ex)
                    {
                        Console.WriteLine($"[LoginServer] Unhandled client error: {ex.Message}");
                    }
                }, cancellationToken);
            }
            catch (ObjectDisposedException)
            {
                // Server is shutting down
                break;
            }
            catch (Exception ex) when (_running)
            {
                Console.WriteLine($"[LoginServer] Error accepting client: {ex.Message}");
                await Task.Delay(100, cancellationToken); // Brief delay before retry
            }
        }
    }

    public Task StopAsync()
    {
        if (!_running)
        {
            return Task.CompletedTask;
        }

        Console.WriteLine("[LoginServer] Shutting down...");
        _running = false;
        _cancellationTokenSource.Cancel();
        _listener?.Stop();
        Console.WriteLine("[LoginServer] Shutdown complete");

        return Task.CompletedTask;
    }
}
