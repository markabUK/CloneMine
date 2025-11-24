# CloneMine Client-Server Architecture

## Overview

CloneMine now supports a client-server multiplayer architecture in addition to the original single-player mode. The server handles all game logic, world state, and player management, while clients connect to render and provide input.

## Building

### Server (No Vulkan Required)

The server can be built without Vulkan:

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

This will create the `CloneMineServer` executable in `build/src/`.

### Client (Requires Vulkan SDK)

If Vulkan SDK is installed, the build will also create `CloneMineClient` and the original `CloneMine` executables:

```bash
# Install Vulkan SDK first (platform-specific)
# Then build as above
```

## Running the Server

Start the server with default settings (port 25565):

```bash
./CloneMineServer
```

Or specify a custom port:

```bash
./CloneMineServer 12345
```

The server will:
- Listen for client connections on the specified port
- Maintain authoritative game state
- Handle multiple simultaneous clients
- Auto-save player data to `server_saves/`
- Update all clients with world and player state at 60 Hz

### Server Controls

- `Ctrl+C` or `SIGTERM` to gracefully shut down the server
- Server saves all player data on shutdown

## Running the Client

Connect to a server:

```bash
./CloneMineClient <hostname> <port> <player_name>
```

For example:

```bash
./CloneMineClient localhost 25565 Alice
./CloneMineClient example.com 12345 Bob
```

The client will:
- Connect to the specified server
- Send player input (movement, actions)
- Render the game world and other players
- Display your character's health and resources

## Architecture

### Network Protocol

Communication uses TCP sockets with a simple binary protocol:

1. **Connect Request**: Client sends player name
2. **Connect Response**: Server assigns player ID
3. **Player Input**: Client sends movement/actions 30x/sec
4. **State Updates**: Server broadcasts player states 60x/sec
5. **Disconnect**: Graceful disconnection with save

### Message Types

- `CONNECT_REQUEST` / `CONNECT_RESPONSE` - Connection handshake
- `PLAYER_INPUT` - Client movement and actions
- `PLAYER_STATE_UPDATE` - Authoritative player positions
- `PLAYER_SPAWN` / `PLAYER_DESPAWN` - Player join/leave
- `WORLD_STATE` / `BLOCK_UPDATE` / `CHUNK_DATA` - World changes
- `CHAT_MESSAGE` - Player communication

### Server Architecture

```
GameServer
├── World (authoritative game state)
├── ServerPlayer[] (connected players)
│   ├── Player (character data)
│   └── TCP Socket (network connection)
└── Save System (persistent storage)
```

### Client Architecture

```
ClientApplication
├── NetworkClient (connection to server)
├── Renderer (Vulkan graphics)
├── World (client-side prediction)
├── Local Player (input prediction)
└── Remote Players[] (other clients)
```

## Player Data Persistence

The server automatically saves player data:

- Location: `server_saves/<player_name>/player.dat`
- Saved on: Disconnect, server shutdown
- Loaded on: Player connection (if exists)
- Data includes:
  - Position and orientation
  - Character class and level
  - Health, mana/energy/focus
  - Stats (STR, DEX, CON, INT, WIS, CHA)
  - Experience points
  - Inventory (coming soon)

## Features

### Implemented

- ✅ TCP network communication
- ✅ Multiple simultaneous clients
- ✅ Server-authoritative game logic
- ✅ Player state synchronization
- ✅ Player spawn/despawn notifications
- ✅ Persistent player data (saves/loads)
- ✅ Graceful connection/disconnection
- ✅ Character stats and leveling
- ✅ Health and resource systems

### Coming Soon

- ⏳ Client input handling (keyboard/mouse)
- ⏳ Block breaking/placing via network
- ⏳ Chat system
- ⏳ Combat between players
- ⏳ Inventory synchronization
- ⏳ Quest system
- ⏳ World persistence (blocks/chunks)

## Development

### Adding New Messages

1. Define message type in `network/NetworkMessage.h`
2. Implement serialization in `network/NetworkMessage.cpp`
3. Handle in server: `server/GameServer.cpp::handlePlayerInput`
4. Handle in client: `client/ClientApplication.cpp::handleNetworkMessage`

### Testing

Run server in one terminal:
```bash
./CloneMineServer 25565
```

Test with netcat:
```bash
nc localhost 25565
```

Or run multiple clients in separate terminals.

## Security Considerations

- No authentication yet - use behind firewall
- No encryption - don't send sensitive data
- Input validation on server side
- Sandbox Lua plugins on both client/server
- Rate limiting needed for production

## Performance

- Server tick rate: 60 Hz
- Client send rate: 30 Hz
- Typical bandwidth: ~10 KB/s per client
- Server can handle 50+ simultaneous players (estimated)

## Troubleshooting

**Server won't start:**
- Check if port is already in use: `netstat -tuln | grep <port>`
- Use a different port number

**Client can't connect:**
- Verify server is running and listening
- Check firewall settings
- Try connecting to localhost first

**Player data not saving:**
- Ensure `server_saves/` directory is writable
- Check server console for error messages

**Lag or desync:**
- Check network latency: `ping <server>`
- Server may be overloaded
- Increase tick rate or reduce player count

## Backward Compatibility

The original single-player mode is preserved:

```bash
./CloneMine  # Original single-player game
```

All existing features work as before.
