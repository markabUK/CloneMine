using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Collections.Concurrent;

namespace CloneMine.AuctionServer;

class AuctionItem
{
    public int AuctionId { get; set; }
    public string Seller { get; set; } = string.Empty;
    public string ItemName { get; set; } = string.Empty;
    public int Quantity { get; set; }
    public int StartingPrice { get; set; }
    public int CurrentBid { get; set; }
    public string? CurrentBidder { get; set; }
    public DateTime ExpirationTime { get; set; }
    public bool IsActive { get; set; }
}

class Program
{
    private static bool _running = true;
    private static TcpListener? _listener;
    private static readonly ConcurrentDictionary<int, AuctionItem> _activeAuctions = new();
    private static int _nextAuctionId = 1;
    private static int _totalProcessed = 0;
    
    static async Task Main(string[] args)
    {
        Console.WriteLine("==================================");
        Console.WriteLine("  CloneMine Auction Server (.NET 10)");
        Console.WriteLine("==================================");
        Console.WriteLine();
        
        // Parse command line arguments
        int port = 25569; // Default auction port
        string configFile = "server_config.txt";
        
        if (args.Length > 0 && int.TryParse(args[0], out int parsedPort))
        {
            port = parsedPort;
        }
        
        if (args.Length > 1)
        {
            configFile = args[1];
        }
        
        Console.WriteLine("[AuctionServer] Configuration:");
        Console.WriteLine($"  Port: {port}");
        Console.WriteLine($"  Config file: {configFile}");
        Console.WriteLine();
        
        // Setup signal handlers
        Console.CancelKeyPress += (sender, e) =>
        {
            e.Cancel = true;
            Console.WriteLine("\n[AuctionServer] Shutdown signal received");
            _running = false;
        };
        
        try
        {
            // Start TCP listener
            _listener = new TcpListener(IPAddress.Any, port);
            _listener.Start();
            
            Console.WriteLine($"[AuctionServer] Started on port {port}");
            Console.WriteLine("[AuctionServer] Press Ctrl+C to stop");
            Console.WriteLine();
            
            // Accept connections asynchronously
            _ = Task.Run(AcceptClientsAsync);
            
            // Start auction expiration checker
            _ = Task.Run(CheckExpirationsAsync);
            
            // Status reporting
            _ = Task.Run(ReportStatusAsync);
            
            // Main loop
            while (_running)
            {
                await Task.Delay(100);
            }
        }
        catch (Exception ex)
        {
            Console.WriteLine($"[AuctionServer] Fatal error: {ex.Message}");
            return;
        }
        finally
        {
            // Shutdown
            Console.WriteLine("[AuctionServer] Shutting down...");
            _listener?.Stop();
            Console.WriteLine($"[AuctionServer] Final stats:");
            Console.WriteLine($"  Active auctions: {_activeAuctions.Count}");
            Console.WriteLine($"  Total processed: {_totalProcessed}");
            Console.WriteLine("[AuctionServer] Shutdown complete");
        }
    }
    
    private static async Task AcceptClientsAsync()
    {
        while (_running && _listener != null)
        {
            try
            {
                var client = await _listener.AcceptTcpClientAsync();
                Console.WriteLine($"[AuctionServer] Client connected: {client.Client.RemoteEndPoint}");
                
                // Handle client in separate task
                _ = Task.Run(() => HandleClientAsync(client));
            }
            catch (Exception ex) when (_running)
            {
                Console.WriteLine($"[AuctionServer] Error accepting client: {ex.Message}");
            }
        }
    }
    
    private static async Task HandleClientAsync(TcpClient client)
    {
        try
        {
            using var stream = client.GetStream();
            var buffer = new byte[2048];
            
            while (_running && client.Connected)
            {
                var bytesRead = await stream.ReadAsync(buffer, 0, buffer.Length);
                if (bytesRead == 0) break;
                
                var message = Encoding.UTF8.GetString(buffer, 0, bytesRead).Trim();
                var response = ProcessMessage(message);
                
                var responseBytes = Encoding.UTF8.GetBytes(response + "\n");
                await stream.WriteAsync(responseBytes, 0, responseBytes.Length);
            }
        }
        catch (Exception ex)
        {
            Console.WriteLine($"[AuctionServer] Client error: {ex.Message}");
        }
        finally
        {
            client.Close();
        }
    }
    
    private static string ProcessMessage(string message)
    {
        var parts = message.Split('|');
        if (parts.Length < 1) return "ERROR|Invalid message format";
        
        var command = parts[0].ToUpper();
        
        return command switch
        {
            "LIST_AUCTIONS" => ProcessListAuctions(),
            "CREATE_AUCTION" when parts.Length >= 5 => ProcessCreateAuction(parts[1], parts[2], parts[3], parts[4]),
            "PLACE_BID" when parts.Length >= 3 => ProcessPlaceBid(parts[1], parts[2]),
            "GET_AUCTION" when parts.Length >= 2 => ProcessGetAuction(parts[1]),
            "CANCEL_AUCTION" when parts.Length >= 3 => ProcessCancelAuction(parts[1], parts[2]),
            "GET_MY_AUCTIONS" when parts.Length >= 2 => ProcessGetMyAuctions(parts[1]),
            _ => "ERROR|Unknown command"
        };
    }
    
    private static string ProcessListAuctions()
    {
        var activeAuctions = _activeAuctions.Values.Where(a => a.IsActive).ToList();
        
        if (activeAuctions.Count == 0)
        {
            return "AUCTIONS|NONE";
        }
        
        var auctionList = string.Join(";", activeAuctions.Select(a =>
            $"{a.AuctionId}:{a.ItemName}:{a.Quantity}:{a.CurrentBid}:{a.Seller}"));
        
        return $"AUCTIONS|{auctionList}";
    }
    
    private static string ProcessCreateAuction(string seller, string itemName, string quantityStr, string priceStr)
    {
        if (!int.TryParse(quantityStr, out int quantity) || quantity <= 0)
        {
            return "ERROR|Invalid quantity";
        }
        
        if (!int.TryParse(priceStr, out int price) || price <= 0)
        {
            return "ERROR|Invalid price";
        }
        
        var auction = new AuctionItem
        {
            AuctionId = _nextAuctionId++,
            Seller = seller,
            ItemName = itemName,
            Quantity = quantity,
            StartingPrice = price,
            CurrentBid = price,
            CurrentBidder = null,
            ExpirationTime = DateTime.Now.AddHours(24),
            IsActive = true
        };
        
        _activeAuctions[auction.AuctionId] = auction;
        
        Console.WriteLine($"[AuctionServer] Auction created: #{auction.AuctionId} - {itemName} x{quantity} by {seller} ({price} gold)");
        return $"SUCCESS|Auction created: #{auction.AuctionId}";
    }
    
    private static string ProcessPlaceBid(string auctionIdStr, string bidderData)
    {
        if (!int.TryParse(auctionIdStr, out int auctionId))
        {
            return "ERROR|Invalid auction ID";
        }
        
        if (!_activeAuctions.TryGetValue(auctionId, out var auction))
        {
            return "ERROR|Auction not found";
        }
        
        if (!auction.IsActive)
        {
            return "ERROR|Auction is not active";
        }
        
        var bidParts = bidderData.Split(':');
        if (bidParts.Length != 2)
        {
            return "ERROR|Invalid bid data";
        }
        
        var bidder = bidParts[0];
        if (!int.TryParse(bidParts[1], out int bidAmount))
        {
            return "ERROR|Invalid bid amount";
        }
        
        if (bidder == auction.Seller)
        {
            return "ERROR|Cannot bid on your own auction";
        }
        
        if (bidAmount <= auction.CurrentBid)
        {
            return $"ERROR|Bid must be higher than current bid ({auction.CurrentBid} gold)";
        }
        
        auction.CurrentBid = bidAmount;
        auction.CurrentBidder = bidder;
        
        Console.WriteLine($"[AuctionServer] New bid on #{auctionId}: {bidAmount} gold by {bidder}");
        return $"SUCCESS|Bid placed: {bidAmount} gold";
    }
    
    private static string ProcessGetAuction(string auctionIdStr)
    {
        if (!int.TryParse(auctionIdStr, out int auctionId))
        {
            return "ERROR|Invalid auction ID";
        }
        
        if (!_activeAuctions.TryGetValue(auctionId, out var auction))
        {
            return "ERROR|Auction not found";
        }
        
        var timeRemaining = auction.ExpirationTime - DateTime.Now;
        var hoursRemaining = (int)Math.Max(0, timeRemaining.TotalHours);
        
        var auctionData = $"{auction.AuctionId}|{auction.ItemName}|{auction.Quantity}|" +
                         $"{auction.StartingPrice}|{auction.CurrentBid}|{auction.CurrentBidder ?? "None"}|" +
                         $"{auction.Seller}|{hoursRemaining}|{auction.IsActive}";
        
        return $"AUCTION|{auctionData}";
    }
    
    private static string ProcessCancelAuction(string seller, string auctionIdStr)
    {
        if (!int.TryParse(auctionIdStr, out int auctionId))
        {
            return "ERROR|Invalid auction ID";
        }
        
        if (!_activeAuctions.TryGetValue(auctionId, out var auction))
        {
            return "ERROR|Auction not found";
        }
        
        if (auction.Seller != seller)
        {
            return "ERROR|You can only cancel your own auctions";
        }
        
        if (auction.CurrentBidder != null)
        {
            return "ERROR|Cannot cancel auction with active bids";
        }
        
        auction.IsActive = false;
        _totalProcessed++;
        
        Console.WriteLine($"[AuctionServer] Auction cancelled: #{auctionId} by {seller}");
        return $"SUCCESS|Auction cancelled";
    }
    
    private static string ProcessGetMyAuctions(string username)
    {
        var myAuctions = _activeAuctions.Values
            .Where(a => a.Seller == username && a.IsActive)
            .ToList();
        
        if (myAuctions.Count == 0)
        {
            return "MY_AUCTIONS|NONE";
        }
        
        var auctionList = string.Join(";", myAuctions.Select(a =>
            $"{a.AuctionId}:{a.ItemName}:{a.Quantity}:{a.CurrentBid}"));
        
        return $"MY_AUCTIONS|{auctionList}";
    }
    
    private static async Task CheckExpirationsAsync()
    {
        while (_running)
        {
            try
            {
                var now = DateTime.Now;
                var expiredAuctions = _activeAuctions.Values
                    .Where(a => a.IsActive && a.ExpirationTime <= now)
                    .ToList();
                
                foreach (var auction in expiredAuctions)
                {
                    auction.IsActive = false;
                    _totalProcessed++;
                    
                    if (auction.CurrentBidder != null)
                    {
                        Console.WriteLine($"[AuctionServer] Auction #{auction.AuctionId} completed: " +
                                        $"{auction.ItemName} sold to {auction.CurrentBidder} for {auction.CurrentBid} gold");
                    }
                    else
                    {
                        Console.WriteLine($"[AuctionServer] Auction #{auction.AuctionId} expired with no bids");
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"[AuctionServer] Error checking expirations: {ex.Message}");
            }
            
            await Task.Delay(60000); // Check every minute
        }
    }
    
    private static async Task ReportStatusAsync()
    {
        while (_running)
        {
            await Task.Delay(60000); // Report every minute
            
            var activeCount = _activeAuctions.Values.Count(a => a.IsActive);
            Console.WriteLine($"[AuctionServer] Status - Active: {activeCount}, Total processed: {_totalProcessed}");
        }
    }
}
