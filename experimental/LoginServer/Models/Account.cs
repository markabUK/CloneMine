namespace CloneMine.LoginServer.Models;

/// <summary>
/// Represents a user account in the system
/// </summary>
public class Account
{
    public string Username { get; set; } = string.Empty;
    public string PasswordHash { get; set; } = string.Empty;
    public int CharacterCount { get; set; }
    public DateTime CreatedAt { get; set; }
    public DateTime LastLogin { get; set; }
}
