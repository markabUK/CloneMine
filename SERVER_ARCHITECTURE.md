# Server Architecture Guide

## Overview

CloneMine now uses a microservices architecture with three separate servers:

1. **Game Server** (Port 25565) - Main game logic, world state, player physics
2. **Chat Server** (Port 25566) - All chat communication and history
3. **Quest Server** (Port 25567) - Quest management and progress tracking

This separation provides:
- Better performance (isolated workloads)
- Enhanced security (attack surface reduction)
- Scalability (independent scaling)
- Maintainability (focused codebases)

## Starting the Servers

### All Servers Together
```bash
# Terminal 1 - Game Server
./CloneMineServer 25565

# Terminal 2 - Chat Server
./CloneMineChatServer 25566

# Terminal 3 - Quest Server
./CloneMineQuestServer 25567
```

### Single Server (for testing)
```bash
# Just game server
./CloneMineServer

# Just chat server
./CloneMineChatServer

# Just quest server
./CloneMineQuestServer
```

## Game Server (Port 25565)

### Responsibilities
- Player physics and movement
- World state (blocks, chunks)
- Monster AI and combat
- Item drops and pickups
- Player authentication
- Save/load player data

### Configuration
- Default port: 25565
- Tick rate: 60 Hz
- Max players: Configurable (default 50)
- Save directory: `server_saves/`

### Network Protocol
- Encrypted TCP connections
- Size-prefixed binary messages
- Packet validation on all messages
- Messages: CONNECT, DISCONNECT, PLAYER_INPUT, PLAYER_STATE_UPDATE, PLAYER_SPAWN

## Chat Server (Port 25566)

### Responsibilities
- All chat message routing
- Chat history (last 100 messages)
- Player authentication for chat
- Message broadcasting
- Spam filtering (future)

### Features
- **Independent Operation**: Can run without game server
- **Chat History**: New connections receive recent messages
- **Encrypted**: All chat encrypted with XOR cipher
- **Validated**: Packet validation prevents injection
- **Persistent**: History survives server restart (future)

### Configuration
- Default port: 25566
- Max history: 100 messages
- Max message length: 256 characters
- Broadcast to all connected clients

### Network Protocol
- Separate connection from game server
- Encrypted TCP with packet validation
- Messages: CHAT_MESSAGE
- Sender name validation (can't impersonate)

### Usage Example
```bash
# Client connects to both:
# Game Server: localhost:25565
# Chat Server: localhost:25566

# Press T to chat
# Type message
# Press ENTER to send
```

## Quest Server (Port 25567)

### Responsibilities
- Quest database management
- Player quest progress tracking
- Quest completion detection
- Reward distribution coordination
- Quest availability based on level

### Quest Types Supported
1. **KILL_MONSTERS** - Defeat N specific monsters
2. **COLLECT_ITEMS** - Gather N specific items
3. **EXPLORE_LOCATION** - Discover specific areas
4. **TALK_TO_NPC** - Interact with NPCs
5. **CRAFT_ITEM** - Create specific items
6. **COMBINATION** - Multiple objectives

### Sample Quests

**Quest 1: Zombie Slayer I**
- Level: 1 (Recommended: 3)
- Objective: Kill 10 zombies
- Reward: 100 XP, 50 gold, health potion

**Quest 2: Gather Resources**
- Level: 1 (Recommended: 1)
- Objective: Collect 20 wood blocks
- Reward: 50 XP, 25 gold

**Quest 3: Dungeon Explorer**
- Level: 5 (Recommended: 8)
- Objectives:
  - Explore Dark Cavern
  - Defeat Cave Troll (boss)
- Reward: 500 XP, 250 gold, rare sword, magic ring

### Configuration
- Default port: 25567
- Quest database: In-memory (future: SQL/JSON)
- Progress tracking: Real-time
- Completion rewards: Coordinated with game server

### Network Protocol
- Encrypted TCP connection
- Packet validation
- Messages: QUEST_REQUEST, QUEST_PROGRESS, QUEST_COMPLETE
- Progress updates from game server

### Integration

The Quest Server needs to communicate with the Game Server to track progress:

```
Player kills zombie -> Game Server
Game Server -> Quest Server: "Player X killed zombie"
Quest Server -> Updates progress (1/10 zombies)
Quest Server -> Client: Updated progress
```

## Packet Validation

All servers now validate incoming packets:

### Validation Checks
1. **Size Validation**
   - Minimum size per message type
   - Maximum size per message type
   - Prevents buffer overflow

2. **Type Validation**
   - Message type matches expected
   - Prevents protocol confusion attacks

3. **Data Validation**
   - No nulls in critical data
   - String lengths reasonable
   - Prevents injection attacks

4. **Integrity Checks**
   - Size-prefixed messages
   - Data doesn't extend beyond packet
   - Prevents malformed packet crashes

### Validation Results
- `VALID` - Packet passes all checks
- `INVALID_SIZE` - Size out of bounds
- `INVALID_TYPE` - Wrong message type
- `INVALID_DATA` - Data structure invalid
- `NULL_DATA` - Null bytes in critical sections
- `SUSPICIOUS` - Unusual patterns detected

Rejected packets are logged and connection may be closed.

## Security Features

### Per-Server Security

**Game Server:**
- Validates all player input
- Server-authoritative physics
- Rate limiting (future)
- Anti-cheat checks (future)

**Chat Server:**
- Sender name validation
- Message length limits
- Spam filtering (future)
- Profanity filter (future)

**Quest Server:**
- Progress validation
- Reward verification
- Anti-exploit checks
- Level requirement enforcement

### Network Security
- All packets encrypted
- All packets validated
- Size limits enforced
- Connection rate limiting (future)

## Performance Characteristics

### Game Server
- Tick rate: 60 Hz
- Updates/sec: 60
- Network: ~5 KB/s per player
- CPU: Moderate (physics, AI)

### Chat Server
- Event-driven (no fixed rate)
- Network: <1 KB/s per player average
- CPU: Very low
- Can handle 1000+ clients

### Quest Server
- Event-driven (no fixed rate)
- Network: Minimal (quest queries only)
- CPU: Very low
- Can handle 1000+ clients

## Scalability

### Horizontal Scaling

**Game Server:**
- Run multiple instances
- Assign players to instances
- Cross-instance chat via Chat Server

**Chat Server:**
- Single instance handles all players
- Can distribute by channel (future)

**Quest Server:**
- Single instance sufficient
- Can shard by player ID (future)

### Load Balancing
```
                  [Load Balancer]
                       |
        +--------------+--------------+
        |              |              |
   Game Server 1  Game Server 2  Game Server 3
   (50 players)   (50 players)   (50 players)
        |              |              |
        +-------+------+------+-------+
                |             |
         Chat Server    Quest Server
         (All players)  (All players)
```

## Monitoring

### Health Checks
```bash
# Check if servers are running
netstat -tuln | grep 25565  # Game
netstat -tuln | grep 25566  # Chat
netstat -tuln | grep 25567  # Quest
```

### Logs
- Game Server: Shows player connections, game events
- Chat Server: Shows all chat messages
- Quest Server: Shows quest completions, progress updates

## Troubleshooting

### Port Already in Use
```bash
# Find process using port
lsof -i :25565

# Kill process
kill <PID>

# Or use different port
./CloneMineServer 12345
```

### Server Crashes
1. Check logs for error messages
2. Verify all dependencies installed
3. Check system resources (RAM, CPU)
4. Restart server

### Connection Issues
1. Verify server is running
2. Check firewall settings
3. Test with telnet: `telnet localhost 25565`
4. Verify client connects to correct ports

### Performance Issues
1. Monitor CPU/RAM usage
2. Reduce tick rate if needed
3. Limit max players
4. Run profiler for bottlenecks

## Development

### Adding New Quest Types
```cpp
// In QuestData.h
enum class QuestType {
    // ... existing types
    BUILD_STRUCTURE  // New type
};

// In QuestServer.cpp
quest::Quest quest4;
quest4.questId = 4;
quest4.title = "Master Builder";
// ... configure quest

addQuest(quest4);
```

### Adding New Chat Commands
```cpp
// In ChatServer.cpp handleChatMessage
if (message.starts_with("/")) {
    handleCommand(sender, message);
    return;
}
```

### Custom Validation Rules
```cpp
// In PacketValidator.cpp
bool PacketValidator::validateCustom(const std::vector<uint8_t>& data) {
    // Add custom validation logic
    return true;
}
```

## Best Practices

1. **Always run all three servers** for full functionality
2. **Use separate machines** in production for isolation
3. **Monitor logs** for suspicious activity
4. **Backup quest data** regularly
5. **Test with multiple clients** before deployment
6. **Use firewall rules** to restrict access
7. **Enable rate limiting** in production
8. **Implement authentication** for admin commands

## Future Enhancements

- [ ] Database backend for quests (PostgreSQL/MySQL)
- [ ] Redis for chat history persistence
- [ ] WebSocket support for web clients
- [ ] Admin dashboard for monitoring
- [ ] Automatic server discovery
- [ ] Cross-server private messaging
- [ ] Guild/clan chat channels
- [ ] Voice chat server
- [ ] Achievement server
- [ ] Leaderboard server

## Conclusion

The microservices architecture provides a robust, scalable foundation for CloneMine's multiplayer system. Each server can be developed, deployed, and scaled independently while working together to provide a seamless gaming experience.
