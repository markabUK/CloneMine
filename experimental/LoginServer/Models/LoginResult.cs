namespace CloneMine.LoginServer.Models;

/// <summary>
/// Result of a login attempt
/// </summary>
public class LoginResult
{
    public bool Success { get; set; }
    public string Message { get; set; } = string.Empty;
    public Account? Account { get; set; }
}
