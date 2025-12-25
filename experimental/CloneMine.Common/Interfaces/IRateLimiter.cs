namespace CloneMine.Common.Interfaces;

/// <summary>
/// Interface for rate limiting to prevent DOS attacks
/// </summary>
public interface IRateLimiter
{
    /// <summary>
    /// Check if a request from the given IP address should be allowed
    /// </summary>
    /// <param name="ipAddress">The IP address making the request</param>
    /// <returns>True if allowed, false if rate limit exceeded</returns>
    bool AllowRequest(string ipAddress);
    
    /// <summary>
    /// Reset rate limit for a specific IP address
    /// </summary>
    /// <param name="ipAddress">The IP address to reset</param>
    void ResetIp(string ipAddress);
    
    /// <summary>
    /// Clear all rate limit data
    /// </summary>
    void ClearAll();
    
    /// <summary>
    /// Get current request count for an IP address
    /// </summary>
    /// <param name="ipAddress">The IP address to query</param>
    /// <returns>Number of requests in current window</returns>
    int GetRequestCount(string ipAddress);
}
