using System.Net;
using System.Net.Sockets;
using CloneMine.GameServer.Interfaces;
using CloneMine.GameServer.Models;

namespace CloneMine.GameServer.Services;

public class TcpServerListener
{
    private readonly ServerConfiguration _config;
    private readonly IClientHandler _clientHandler;
    private readonly IGameService _gameService;
    private TcpListener? _listener;
    private bool _running;
    private readonly CancellationTokenSource _cancellationTokenSource;

    public bool IsRunning => _running;

    public TcpServerListener(
        ServerConfiguration config,
        IClientHandler clientHandler,
        IGameService gameService)
    {
        _config = config ?? throw new ArgumentNullException(nameof(config));
        _clientHandler = clientHandler ?? throw new ArgumentNullException(nameof(clientHandler));
        _gameService = gameService ?? throw new ArgumentNullException(nameof(gameService));
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

            Console.WriteLine($"[GameServer] Started on port {port}");
            Console.WriteLine($"[GameServer] Tick rate: {_config.TickRate} Hz");
            Console.WriteLine("[GameServer] Press Ctrl+C to stop");
            Console.WriteLine();

            _ = Task.Run(() => GameLoopAsync(_cancellationTokenSource.Token));

            await AcceptClientsAsync(_cancellationTokenSource.Token);
        }
        catch (Exception ex)
        {
            Console.WriteLine($"[GameServer] Failed to start: {ex.Message}");
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
                        Console.WriteLine($"[GameServer] Unhandled client error: {ex.Message}");
                    }
                }, cancellationToken);
            }
            catch (ObjectDisposedException)
            {
                break;
            }
            catch (Exception ex) when (_running)
            {
                Console.WriteLine($"[GameServer] Error accepting client: {ex.Message}");
                await Task.Delay(100, cancellationToken);
            }
        }
    }

    private async Task GameLoopAsync(CancellationToken cancellationToken)
    {
        var tickInterval = TimeSpan.FromSeconds(_config.TickInterval);

        while (_running && !cancellationToken.IsCancellationRequested)
        {
            try
            {
                await _gameService.UpdateGameStateAsync();
                await Task.Delay(tickInterval, cancellationToken);
            }
            catch (TaskCanceledException)
            {
                break;
            }
            catch (Exception ex)
            {
                Console.WriteLine($"[GameServer] Error in game loop: {ex.Message}");
            }
        }
    }

    public Task StopAsync()
    {
        if (!_running) return Task.CompletedTask;

        Console.WriteLine("[GameServer] Shutting down...");
        _running = false;
        _cancellationTokenSource.Cancel();
        _listener?.Stop();
        Console.WriteLine("[GameServer] Shutdown complete");

        return Task.CompletedTask;
    }
}
