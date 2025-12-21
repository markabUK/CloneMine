using System.Net;
using System.Net.Sockets;
using CloneMine.AuctionServer.Interfaces;
using CloneMine.AuctionServer.Models;

namespace CloneMine.AuctionServer.Services;

/// <summary>
/// TCP server listener
/// Single Responsibility: Managing server lifecycle and accepting connections
/// </summary>
public class TcpServerListener
{
    private readonly ServerConfiguration _config;
    private readonly IClientHandler _clientHandler;
    private readonly IAuctionService _auctionService;
    private TcpListener? _listener;
    private bool _running;
    private readonly CancellationTokenSource _cancellationTokenSource;

    public bool IsRunning => _running;

    public TcpServerListener(
        ServerConfiguration config,
        IClientHandler clientHandler,
        IAuctionService auctionService)
    {
        _config = config ?? throw new ArgumentNullException(nameof(config));
        _clientHandler = clientHandler ?? throw new ArgumentNullException(nameof(clientHandler));
        _auctionService = auctionService ?? throw new ArgumentNullException(nameof(auctionService));
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

            Console.WriteLine($"[AuctionServer] Started on port {port}");
            Console.WriteLine("[AuctionServer] Press Ctrl+C to stop");
            Console.WriteLine();

            // Start background tasks
            _ = Task.Run(() => AcceptClientsAsync(_cancellationTokenSource.Token));
            _ = Task.Run(() => CheckExpirationsAsync(_cancellationTokenSource.Token));
            _ = Task.Run(() => ReportStatusAsync(_cancellationTokenSource.Token));

            // Keep running
            while (_running && !_cancellationTokenSource.Token.IsCancellationRequested)
            {
                await Task.Delay(100, _cancellationTokenSource.Token);
            }
        }
        catch (Exception ex)
        {
            Console.WriteLine($"[AuctionServer] Failed to start: {ex.Message}");
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
                        Console.WriteLine($"[AuctionServer] Unhandled client error: {ex.Message}");
                    }
                }, cancellationToken);
            }
            catch (ObjectDisposedException)
            {
                break;
            }
            catch (Exception ex) when (_running)
            {
                Console.WriteLine($"[AuctionServer] Error accepting client: {ex.Message}");
                await Task.Delay(100, cancellationToken);
            }
        }
    }

    private async Task CheckExpirationsAsync(CancellationToken cancellationToken)
    {
        while (_running && !cancellationToken.IsCancellationRequested)
        {
            try
            {
                await Task.Delay(60000, cancellationToken); // Check every minute
                await _auctionService.CheckExpirationsAsync();
            }
            catch (TaskCanceledException)
            {
                break;
            }
            catch (Exception ex)
            {
                Console.WriteLine($"[AuctionServer] Error checking expirations: {ex.Message}");
            }
        }
    }

    private async Task ReportStatusAsync(CancellationToken cancellationToken)
    {
        while (_running && !cancellationToken.IsCancellationRequested)
        {
            try
            {
                await Task.Delay(60000, cancellationToken); // Report every minute
                var activeAuctions = await _auctionService.GetActiveAuctionsAsync();
                var activeCount = activeAuctions.Count();
                Console.WriteLine($"[AuctionServer] Status - Active auctions: {activeCount}");
            }
            catch (TaskCanceledException)
            {
                break;
            }
            catch (Exception ex)
            {
                Console.WriteLine($"[AuctionServer] Error reporting status: {ex.Message}");
            }
        }
    }

    public Task StopAsync()
    {
        if (!_running)
        {
            return Task.CompletedTask;
        }

        Console.WriteLine("[AuctionServer] Shutting down...");
        _running = false;
        _cancellationTokenSource.Cancel();
        _listener?.Stop();
        Console.WriteLine("[AuctionServer] Shutdown complete");

        return Task.CompletedTask;
    }
}
