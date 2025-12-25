using System.Collections.Concurrent;
using CloneMine.Common.Interfaces;

namespace CloneMine.Common.Security;

/// <summary>
/// Rate limiter using token bucket algorithm to prevent DOS attacks
/// Single Responsibility: Managing request rate limits per IP address
/// </summary>
public class RateLimiter : IRateLimiter
{
    private const int CleanupMultiplier = 2; // Remove entries older than 2x window duration
    
    private readonly int _maxRequests;
    private readonly TimeSpan _windowDuration;
    private readonly ConcurrentDictionary<string, ClientInfo> _clients;
    private DateTime _lastCleanup;
    private readonly object _cleanupLock = new object();

    private class ClientInfo
    {
        public int RequestCount { get; set; }
        public DateTime WindowStart { get; set; }
        public object Lock { get; } = new object(); // Dedicated lock per client
    }

    /// <summary>
    /// Constructor
    /// </summary>
    /// <param name="maxRequests">Maximum number of requests allowed in the time window (default: 100)</param>
    /// <param name="windowSeconds">Time window in seconds for rate limiting (default: 60)</param>
    public RateLimiter(int maxRequests = 100, int windowSeconds = 60)
    {
        _maxRequests = maxRequests;
        _windowDuration = TimeSpan.FromSeconds(windowSeconds);
        _clients = new ConcurrentDictionary<string, ClientInfo>();
        _lastCleanup = DateTime.UtcNow;
    }

    /// <summary>
    /// Check if a request from the given IP address should be allowed
    /// </summary>
    /// <param name="ipAddress">The IP address making the request</param>
    /// <returns>True if allowed, false if rate limit exceeded</returns>
    public bool AllowRequest(string ipAddress)
    {
        if (string.IsNullOrWhiteSpace(ipAddress))
        {
            return false;
        }

        var now = DateTime.UtcNow;

        // Periodic cleanup of expired entries (every 5 minutes)
        if (now - _lastCleanup > TimeSpan.FromMinutes(5))
        {
            lock (_cleanupLock)
            {
                if (now - _lastCleanup > TimeSpan.FromMinutes(5))
                {
                    CleanupExpiredEntries();
                    _lastCleanup = now;
                }
            }
        }

        var clientInfo = _clients.GetOrAdd(ipAddress, _ => new ClientInfo
        {
            RequestCount = 0,
            WindowStart = now
        });

        lock (clientInfo.Lock)
        {
            var elapsed = now - clientInfo.WindowStart;

            if (elapsed >= _windowDuration)
            {
                // Window expired, reset counter
                clientInfo.RequestCount = 1;
                clientInfo.WindowStart = now;
                return true;
            }

            // Within the window, check if limit exceeded
            if (clientInfo.RequestCount >= _maxRequests)
            {
                Console.WriteLine($"[RateLimiter] Rate limit exceeded for IP: {ipAddress} (requests: {clientInfo.RequestCount})");
                return false;
            }

            // Increment counter
            clientInfo.RequestCount++;
            return true;
        }
    }

    /// <summary>
    /// Reset rate limit for a specific IP address
    /// </summary>
    /// <param name="ipAddress">The IP address to reset</param>
    public void ResetIp(string ipAddress)
    {
        if (!string.IsNullOrWhiteSpace(ipAddress))
        {
            _clients.TryRemove(ipAddress, out _);
        }
    }

    /// <summary>
    /// Clear all rate limit data
    /// </summary>
    public void ClearAll()
    {
        _clients.Clear();
    }

    /// <summary>
    /// Get current request count for an IP address
    /// </summary>
    /// <param name="ipAddress">The IP address to query</param>
    /// <returns>Number of requests in current window</returns>
    public int GetRequestCount(string ipAddress)
    {
        if (string.IsNullOrWhiteSpace(ipAddress))
        {
            return 0;
        }

        if (_clients.TryGetValue(ipAddress, out var clientInfo))
        {
            lock (clientInfo.Lock)
            {
                return clientInfo.RequestCount;
            }
        }

        return 0;
    }

    private void CleanupExpiredEntries()
    {
        var now = DateTime.UtcNow;
        var expiredKeys = new List<string>();

        foreach (var kvp in _clients)
        {
            var elapsed = now - kvp.Value.WindowStart;
            if (elapsed >= _windowDuration * CleanupMultiplier)
            {
                // Remove entries that are expired for more than CleanupMultiplier times the window duration
                expiredKeys.Add(kvp.Key);
            }
        }

        foreach (var key in expiredKeys)
        {
            _clients.TryRemove(key, out _);
        }
    }
}
