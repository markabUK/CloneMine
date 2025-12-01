# CloneMine Multiplayer Quick Start

This guide will help you quickly set up and test the multiplayer functionality.

## Quick Setup

### 1. Build the Server

```bash
git clone --recursive https://github.com/markabUK/CloneMine.git
cd CloneMine
mkdir build && cd build
cmake ..
cmake --build .
```

This creates `CloneMineServer` in `build/src/` directory.

### 2. Start the Server

```bash
cd build/src
./CloneMineServer
```

You should see:
```
CloneMine Server
=================
Initializing game server on port 25565...
Server listening on port 25565
Server main loop started.
```

### 3. Test Connection (Without Client)

In another terminal, test that the server accepts connections:

```bash
telnet localhost 25565
# or
nc localhost 25565
```

You should see "Invalid connect request" in the server logs (expected - proper client sends binary protocol).

Press Ctrl+] then type "quit" to exit telnet, or Ctrl+C for nc.

### 4. Build and Run Client (Requires Vulkan)

If you have Vulkan SDK installed:

```bash
# In the build directory where you built the server
cmake ..  # Reconfigure to detect Vulkan
cmake --build .

# Run the client
cd src
./CloneMineClient localhost 25565 TestPlayer
```

## Testing Multiple Players

### Terminal 1 - Server
```bash
./CloneMineServer 12345
```

### Terminal 2 - First Client
```bash
./CloneMineClient localhost 12345 Alice
```

### Terminal 3 - Second Client
```bash
./CloneMineClient localhost 12345 Bob
```

Both clients will see each other in the game world!

## What's Supported

✅ **Working:**
- Server accepts multiple client connections
- Players spawn in the world
- Server maintains player state (position, health, resources)
- State updates broadcast to all clients
- Player data persists (saves on disconnect)
- Graceful shutdown and reconnection

🚧 **In Progress:**
- Client input handling (movement with keyboard/mouse)
- Block placement/breaking via network
- Chat system
- Combat between players

## Troubleshooting

**Server won't start:**
```bash
# Check if port is in use
netstat -tuln | grep 25565

# Use a different port
./CloneMineServer 12345
```

**Build fails with Vulkan errors:**
```
# Server builds without Vulkan, just skip client
cmake ..
cmake --build . --target CloneMineServer
```

**Client can't connect:**
- Make sure server is running: check "Server listening on port..." message
- Check firewall: `sudo ufw allow 25565` (Linux) or disable temporarily
- Try localhost first: `./CloneMineClient localhost 25565 Test`

## Architecture Overview

```
Server (./CloneMineServer)
  |
  ├─ Manages authoritative game state
  ├─ Handles player connections
  ├─ Broadcasts updates at 60 Hz
  └─ Saves player data
  
Client (./CloneMineClient)
  |
  ├─ Connects to server
  ├─ Sends input at 30 Hz
  ├─ Renders world and players
  └─ Shows local player UI
```

## Next Steps

- Read [MULTIPLAYER.md](MULTIPLAYER.md) for detailed documentation
- Check [IMPLEMENTATION.md](IMPLEMENTATION.md) for architecture details
- Look at source code in `src/server/` and `src/client/`

## Need Help?

- Open an issue on GitHub
- Check server logs in the console
- Enable debug logging (coming soon)

Happy multiplayer gaming! 🎮
