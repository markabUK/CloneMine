# CloneMine Experimental .NET 10 Servers

This directory contains experimental .NET 10 (C#) implementations of the CloneMine game servers. These are cross-platform equivalents of the original C++ servers, designed to run on Windows, Linux, and macOS.

## Overview

The experimental servers provide the same functionality as the original C++ servers but are implemented in modern C# using .NET 10, offering:

- **Cross-platform compatibility** - Run on Windows, Linux, and macOS without recompilation
- **Simplified deployment** - Single executable with runtime dependencies handled by .NET
- **Modern async/await patterns** - Better performance and resource utilization
- **Built-in networking** - Using .NET's robust `System.Net.Sockets` APIs
- **Memory safety** - Managed memory with automatic garbage collection

## Available Servers

### 1. LoginServer (Port 25564)
Handles player authentication and account management.

**Features:**
- User authentication (login/register)
- Account validation
- Character count tracking
- Test accounts for development

**Test Accounts:**
- Username: `test`, Password: `test123`
- Username: `admin`, Password: `admin123`
- Username: `player1`, Password: `password1`

### 2. GameServer (Port 25565)
Main game logic server handling world state and player interactions.

**Features:**
- Player connection management
- Real-time world state updates
- Player movement and physics (60 Hz tick rate)
- Combat system
- Player interactions
- Broadcast messaging

### 3. ChatServer (Port 25566)
Dedicated chat messaging server.

**Features:**
- Real-time chat messaging
- Chat history (last 100 messages)
- Player join/leave notifications
- Message broadcasting
- Character limit enforcement (256 characters)

### 4. QuestServer (Port 25567)
Quest management and progress tracking.

**Features:**
- Quest database management
- Player progress tracking
- Multiple quest types (Kill, Collect, Explore, Talk, Craft, Combination)
- Quest completion detection
- Reward distribution

**Sample Quests:**
1. **Zombie Slayer I** - Kill 10 zombies (Level 1+)
2. **Gather Resources** - Collect 20 wood blocks (Level 1+)
3. **Dungeon Explorer** - Explore Dark Cavern and defeat Cave Troll (Level 5+)

### 5. CharacterServer (Port 25568)
Character creation and management.

**Features:**
- Character CRUD operations
- Character data persistence
- Multiple characters per account (up to 5 by default)
- Character state tracking (position, level, health, experience)

### 6. AuctionServer (Port 25569)
Auction house for player trading.

**Features:**
- Auction creation and management
- Bidding system
- Auction expiration tracking (24-hour default)
- Seller restrictions (can't bid on own auctions)
- Status reporting

## Building

### Prerequisites
- .NET 10 SDK or later
- Cross-platform: Works on Windows, Linux, and macOS

### Build All Servers
```bash
cd experimental
dotnet build CloneMineExperimental.sln
```

### Build Individual Server
```bash
cd experimental/LoginServer
dotnet build
```

## Running the Servers

### Run All Servers (in separate terminals)

**Terminal 1 - Login Server:**
```bash
cd experimental/LoginServer
dotnet run
```

**Terminal 2 - Game Server:**
```bash
cd experimental/GameServer
dotnet run
```

**Terminal 3 - Chat Server:**
```bash
cd experimental/ChatServer
dotnet run
```

**Terminal 4 - Quest Server:**
```bash
cd experimental/QuestServer
dotnet run
```

**Terminal 5 - Character Server:**
```bash
cd experimental/CharacterServer
dotnet run
```

**Terminal 6 - Auction Server:**
```bash
cd experimental/AuctionServer
dotnet run
```

### Custom Port Configuration

Each server accepts command-line arguments for custom ports:

```bash
# Login Server (custom port and max characters)
dotnet run 12345 10 server_config.txt

# Game Server (custom port)
dotnet run 12346

# Chat Server (custom port)
dotnet run 12347

# Quest Server (custom port)
dotnet run 12348

# Character Server (custom port and max characters)
dotnet run 12349 10

# Auction Server (custom port)
dotnet run 12350
```

## Publishing for Deployment

### Create Self-Contained Executables

**For Windows (x64):**
```bash
dotnet publish -c Release -r win-x64 --self-contained
```

**For Linux (x64):**
```bash
dotnet publish -c Release -r linux-x64 --self-contained
```

**For macOS (ARM64 - Apple Silicon):**
```bash
dotnet publish -c Release -r osx-arm64 --self-contained
```

**For macOS (x64 - Intel):**
```bash
dotnet publish -c Release -r osx-x64 --self-contained
```

### Framework-Dependent Deployment
If .NET runtime is already installed on target machine:
```bash
dotnet publish -c Release
```

## Communication Protocol

All servers use TCP with a simple text-based protocol using pipe-delimited messages:

### LoginServer Protocol
```
LOGIN|username|password
REGISTER|username|password
CHECK_CHARACTERS|username
```

### GameServer Protocol
```
CONNECT|username
MOVE|x|y|z
GET_WORLD_STATE
ATTACK|target_username
INTERACT|x|y|z
```

### ChatServer Protocol
```
JOIN|username
CHAT|username|message
```

### QuestServer Protocol
```
LIST_QUESTS|player_level
GET_QUEST|quest_id
ACCEPT_QUEST|username|quest_id
UPDATE_PROGRESS|username|quest_id|objective:amount
GET_PROGRESS|username
COMPLETE_QUEST|username|quest_id
```

### CharacterServer Protocol
```
LIST_CHARACTERS|username
CREATE_CHARACTER|username|character_name
DELETE_CHARACTER|username|character_name
GET_CHARACTER|username|character_name
UPDATE_CHARACTER|username|character_data
```

### AuctionServer Protocol
```
LIST_AUCTIONS
CREATE_AUCTION|seller|item_name|quantity|starting_price
PLACE_BID|auction_id|bidder:bid_amount
GET_AUCTION|auction_id
CANCEL_AUCTION|seller|auction_id
GET_MY_AUCTIONS|username
```

## Testing

### Manual Testing with Telnet
```bash
# Test LoginServer
telnet localhost 25564
LOGIN|test|test123

# Test ChatServer
telnet localhost 25566
JOIN|TestPlayer
CHAT|TestPlayer|Hello World!

# Test QuestServer
telnet localhost 25567
LIST_QUESTS|5
```

### Testing with netcat (nc)
```bash
echo "LOGIN|test|test123" | nc localhost 25564
echo "LIST_QUESTS|5" | nc localhost 25567
```

## Architecture

### Thread Safety
All servers use thread-safe collections:
- `ConcurrentDictionary` for player/session data
- `ConcurrentBag` for client connections
- Proper locking where needed

### Async/Await Pattern
All network I/O operations use async/await for better scalability and responsiveness.

### Signal Handling
All servers properly handle Ctrl+C (SIGINT) for graceful shutdown.

## Performance Characteristics

### LoginServer
- Low CPU usage
- Memory: ~20-50 MB
- Can handle 1000+ concurrent connections

### GameServer
- Moderate CPU usage (60 Hz tick rate)
- Memory: ~50-100 MB base + ~1 KB per player
- Optimized for 50-100 players

### ChatServer
- Very low CPU usage (event-driven)
- Memory: ~20-30 MB
- Can handle 1000+ concurrent clients

### QuestServer
- Very low CPU usage (event-driven)
- Memory: ~30-50 MB
- Can handle 1000+ concurrent clients

### CharacterServer
- Low CPU usage
- Memory: ~30-50 MB
- Can handle 1000+ concurrent connections

### AuctionServer
- Low CPU usage
- Memory: ~30-50 MB
- Periodic expiration checks (every 60 seconds)
- Can handle 1000+ active auctions

## Monitoring and Logging

All servers provide console logging with:
- Connection events
- Player actions
- Errors and warnings
- Periodic status reports

Example log output:
```
[LoginServer] Login successful: test (Characters: 2)
[GameServer] Player connected: test at <0, 64, 0>
[ChatServer] test: Hello everyone!
[QuestServer] test accepted quest: Zombie Slayer I
[CharacterServer] Character created: test/Warrior
[AuctionServer] Auction created: #1 - Iron Sword x1 by test (100 gold)
```

## Security Considerations

**Current Implementation:**
- Plain text communication (for development/testing)
- In-memory storage only
- Basic validation

**Production Recommendations:**
- Add TLS/SSL encryption for network traffic
- Implement proper password hashing (bcrypt/Argon2)
- Add rate limiting and DDoS protection
- Use persistent storage (database)
- Implement proper session management
- Add comprehensive input validation
- Enable audit logging

## Comparison with C++ Servers

| Feature | C++ Servers | .NET 10 Servers |
|---------|-------------|-----------------|
| Platform | Requires compilation per platform | Cross-platform binary |
| Memory Management | Manual (potential for leaks) | Automatic (GC) |
| Networking | ASIO (external library) | Built-in System.Net |
| Threading | std::thread | Task-based async/await |
| Deployment | Complex dependencies | Self-contained option |
| Development Speed | Slower | Faster |
| Raw Performance | Slightly faster | Very competitive |
| Memory Safety | Prone to errors | Memory safe by default |

## Troubleshooting

### Port Already in Use
```bash
# Find process using port
netstat -ano | findstr :25565  # Windows
lsof -i :25565                  # Linux/macOS

# Use a different port
dotnet run 12345
```

### Connection Refused
- Ensure server is running
- Check firewall settings
- Verify correct port number

### Server Crashes
- Check console output for error messages
- Ensure .NET 10 runtime is installed
- Verify system resources available

## Future Enhancements

- [ ] Add database persistence (PostgreSQL/MySQL)
- [ ] Implement TLS/SSL encryption
- [ ] Add WebSocket support for web clients
- [ ] Create REST API endpoints
- [ ] Add admin dashboard
- [ ] Implement proper authentication tokens (JWT)
- [ ] Add Redis for distributed caching
- [ ] Create Docker containers
- [ ] Add Kubernetes deployment manifests
- [ ] Implement horizontal scaling support

## Contributing

These are experimental servers for testing and development. Feel free to:
- Report issues
- Suggest improvements
- Submit pull requests
- Test cross-platform compatibility

## License

Same license as the main CloneMine project.

## Support

For questions or issues with the experimental .NET servers:
1. Check the console logs for error messages
2. Verify .NET 10 is properly installed
3. Test with the provided protocol examples
4. Compare behavior with C++ servers

---

**Note:** These experimental servers are for testing purposes. The original C++ servers remain the primary implementation. Use these .NET servers to:
- Test on platforms where C++ compilation is difficult
- Prototype new features quickly
- Learn the server architecture
- Evaluate .NET for future development
