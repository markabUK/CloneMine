# Distributed Server Deployment Guide

## Overview

CloneMine's microservices architecture supports **distributed deployment** across multiple physical or virtual servers. Each server component can run on a different host, allowing for:

- **Horizontal Scaling**: Distribute load across multiple machines
- **Geographic Distribution**: Place servers closer to players
- **Fault Isolation**: Server failures don't affect other components
- **Independent Updates**: Update services without full system downtime
- **Resource Optimization**: Match hardware to workload

## Architecture

```
┌──────────────┐
│    Client    │
└──────┬───────┘
       │
       ├────────────┐
       │            │
   ┌───▼───┐   ┌───▼────┐
   │ Login │   │  Chat  │
   │Server │   │ Server │
   └───┬───┘   └────────┘
       │
   ┌───▼────────┐
   │ Character  │
   │  Server    │
   └───┬────────┘
       │
   ┌───▼────┐   ┌────────┐
   │  Game  ├───┤ Quest  │
   │ Server │   │ Server │
   └────────┘   └────────┘
```

## Server Components

### 1. Login Server (Port 25564)
- **Purpose**: Authentication and session token generation
- **Dependencies**: None (standalone)
- **Resource Needs**: Low CPU, Low Memory
- **Can run on**: Lightweight VM or container

### 2. Character Server (Port 25568)
- **Purpose**: Character management and authoritative state
- **Dependencies**: None (standalone, but consulted by Game Server)
- **Resource Needs**: Medium CPU, High Memory (stores all character data)
- **Can run on**: Database-optimized server

### 3. Game Server (Port 25565)
- **Purpose**: World simulation, physics, gameplay logic
- **Dependencies**: Queries Character Server, Quest Server
- **Resource Needs**: High CPU, High Memory
- **Can run on**: High-performance compute instance

### 4. Chat Server (Port 25566)
- **Purpose**: Player communication
- **Dependencies**: None (clients connect directly with character names)
- **Resource Needs**: Low CPU, Medium Memory
- **Can run on**: Standard VM

### 5. Quest Server (Port 25567)
- **Purpose**: Quest database and progress tracking
- **Dependencies**: None (queried by Game Server and clients)
- **Resource Needs**: Low CPU, Medium Memory
- **Can run on**: Standard VM

## Configuration File

Create `server_config.txt` with server addresses:

```ini
# Login Server
loginServerHost=auth.example.com
loginServerPort=25564

# Character Server  
characterServerHost=characters.example.com
characterServerPort=25568

# Game Server
gameServerHost=game1.example.com
gameServerPort=25565

# Chat Server
chatServerHost=chat.example.com
chatServerPort=25566

# Quest Server
questServerHost=quests.example.com
questServerPort=25567
```

## Deployment Scenarios

### Scenario 1: All on One Server (Development)
```ini
loginServerHost=localhost
characterServerHost=localhost
gameServerHost=localhost
chatServerHost=localhost
questServerHost=localhost
```

All servers run on ports 25564-25568 on the same machine.

### Scenario 2: Separate Authentication (Basic Production)
```ini
loginServerHost=auth-server.internal
characterServerHost=auth-server.internal
gameServerHost=game-server.public
chatServerHost=game-server.public
questServerHost=game-server.public
```

Auth services isolated, game services together.

### Scenario 3: Full Distribution (High Availability)
```ini
loginServerHost=login1.example.com
characterServerHost=char-db.example.com
gameServerHost=game-us-east.example.com
chatServerHost=chat.example.com
questServerHost=quest-cache.example.com
```

Each service on dedicated infrastructure with geographic optimization.

### Scenario 4: Multi-Region Game Servers
```
Login: auth.example.com:25564 (US-East)
Character: characters.example.com:25568 (US-East)
Game US: game-us.example.com:25565
Game EU: game-eu.example.com:25565  
Game Asia: game-asia.example.com:25565
Chat: chat.example.com:25566 (Global)
Quest: quests.example.com:25567 (Global)
```

Multiple game servers in different regions, all using same character/quest services.

## Server Startup

Each server accepts the config file path:

```bash
# Start each server with configuration
./CloneMineLoginServer 25564 5 server_config.txt
./CloneMineCharacterServer 25568 5 server_config.txt
./CloneMineServer 25565 server_config.txt
./CloneMineChatServer 25566 server_config.txt
./CloneMineQuestServer 25567 server_config.txt
```

## Client Configuration

Clients use the same configuration file:

```bash
./CloneMineClient PlayerName server_config.txt
```

The client will connect to servers based on the configured addresses.

## Network Requirements

### Firewall Rules

**Login Server**:
- Inbound: TCP 25564 from clients

**Character Server**:
- Inbound: TCP 25568 from clients and game servers

**Game Server**:
- Inbound: TCP 25565 from clients
- Outbound: TCP 25568 to character server
- Outbound: TCP 25567 to quest server

**Chat Server**:
- Inbound: TCP 25566 from clients

**Quest Server**:
- Inbound: TCP 25567 from clients and game servers

### Security Considerations

1. **Internal Network**: Place Character/Quest servers on internal network
2. **Public Access**: Only Game/Chat servers need public IPs
3. **Encryption**: All traffic already encrypted (can add TLS)
4. **Authentication**: Use session tokens between services
5. **Rate Limiting**: Implement per-IP connection limits

## Load Balancing

### Game Servers (Horizontal Scaling)

Multiple game servers can share Character/Quest services:

```
                    ┌──────────────┐
                    │  Character   │
                    │   Server     │
                    └───────┬──────┘
                            │
            ┌───────────────┼───────────────┐
            │               │               │
       ┌────▼────┐    ┌────▼────┐    ┌────▼────┐
       │  Game   │    │  Game   │    │  Game   │
       │ Server 1│    │ Server 2│    │ Server 3│
       └─────────┘    └─────────┘    └─────────┘
```

Use a load balancer to distribute clients:
```
[Load Balancer] → {Game Server 1, 2, 3}
```

### Chat Server (Read Replicas)

For high chat volume, use read replicas:
```
[Chat Primary] → [Chat Replica 1]
               → [Chat Replica 2]
```

## Monitoring

Each server logs:
- Startup configuration (shows where other servers are located)
- Connection attempts
- Inter-server communication
- Error conditions

Example output:
```
CloneMine Game Server
=====================

=== Server Configuration ===
Login Server:     auth.example.com:25564
Character Server: char-db.example.com:25568
Game Server:      game-us-east.example.com:25565
Chat Server:      chat.example.com:25566
Quest Server:     quest-cache.example.com:25567
===========================

Starting game server on port 25565...

NOTE: Game server can communicate with:
  - Character Server at char-db.example.com:25568
  - Quest Server at quest-cache.example.com:25567
```

## Docker Deployment

Example `docker-compose.yml` for distributed deployment:

```yaml
version: '3.8'

services:
  login-server:
    image: clonemine-login
    ports:
      - "25564:25564"
    volumes:
      - ./server_config.txt:/app/server_config.txt
    command: ["25564", "5", "server_config.txt"]
    
  character-server:
    image: clonemine-character
    ports:
      - "25568:25568"
    volumes:
      - ./server_config.txt:/app/server_config.txt
      - ./character_data:/app/character_data
    command: ["25568", "5", "server_config.txt"]
    
  game-server:
    image: clonemine-game
    ports:
      - "25565:25565"
    volumes:
      - ./server_config.txt:/app/server_config.txt
    command: ["25565", "server_config.txt"]
    depends_on:
      - character-server
      - quest-server
      
  chat-server:
    image: clonemine-chat
    ports:
      - "25566:25566"
    volumes:
      - ./server_config.txt:/app/server_config.txt
    command: ["25566", "server_config.txt"]
    
  quest-server:
    image: clonemine-quest
    ports:
      - "25567:25567"
    volumes:
      - ./server_config.txt:/app/server_config.txt
    command: ["25567", "server_config.txt"]
```

Deploy to different hosts with Docker Swarm or Kubernetes for true distribution.

## Testing Distributed Setup

1. **Create config** with different hosts:
   ```bash
   cp server_config.txt server_config_distributed.txt
   # Edit to use actual server addresses
   ```

2. **Start servers** on their respective hosts:
   ```bash
   # On auth server
   ./CloneMineLoginServer 25564 5 server_config_distributed.txt
   
   # On character server
   ./CloneMineCharacterServer 25568 5 server_config_distributed.txt
   
   # On game server
   ./CloneMineServer 25565 server_config_distributed.txt
   
   # On chat server
   ./CloneMineChatServer 25566 server_config_distributed.txt
   
   # On quest server
   ./CloneMineQuestServer 25567 server_config_distributed.txt
   ```

3. **Connect client**:
   ```bash
   ./CloneMineClient MyCharacter server_config_distributed.txt
   ```

4. **Verify** server logs show inter-server communication

## Benefits of Distributed Architecture

✅ **Scalability**: Scale each service independently  
✅ **Reliability**: One service failure doesn't crash others  
✅ **Performance**: Optimize hardware per workload  
✅ **Maintenance**: Update services without full downtime  
✅ **Geographic**: Place servers near players (low latency)  
✅ **Security**: Isolate sensitive services (char/auth) from public  
✅ **Cost**: Right-size instances (don't overprovision)  

## Troubleshooting

**Connection Refused**: Check firewall rules and server addresses  
**Timeout**: Verify network connectivity between servers  
**Wrong Server**: Check config file is loaded correctly  
**Auth Failure**: Ensure Login→Character flow is correct  

All servers print their configuration on startup - verify addresses match expectations.
