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


# Notes

You can test the chat server with a quick python script.

You would run the script like this where Alice is the username:

``` shell
python3 chat_client.py Alice 

```

If you spin up several terminals with different users you can see it send a receive the messages


Here is the python script:

``` python
import socket
import struct
import threading
import sys

HOST = '127.0.0.1'
PORT = 25566
SECRET_KEY = "CloneMineSharedSecret2024"

class PacketEncryption:
    """Python implementation matching clonemine::network::PacketEncryption"""
    def __init__(self, secret_key: str):
        self.counter = 0
        self.key = bytearray(32)
        self.derive_key(secret_key)

    def derive_key(self, secret_key: str):
        for i, char in enumerate(secret_key):
            self.key[i % 32] ^= ord(char)
        
        for _ in range(4):
            for i in range(32):
                temp = self.key[i]
                temp = ((temp << 3) & 0xFF) | (temp >> 5)
                temp ^= self.key[(i + 7) % 32]
                self.key[i] = temp

    def get_key_byte(self, index: int) -> int:
        mixed = self.counter ^ ((index * 0x9E3779B9) & 0xFFFFFFFF)
        return self.key[index % 32] ^ (mixed & 0xFF)

    def encrypt(self, data: bytearray):
        for i in range(len(data)):
            data[i] ^= self.get_key_byte(i)
        self.counter = (self.counter + 1) & 0xFFFFFFFF

    def decrypt(self, data: bytearray):
        self.encrypt(data)


def serialize_connect_request(player_id: int, player_name: str) -> bytearray:
    buffer = bytearray()
    CONNECT_REQUEST_TYPE = 0  # CONNECT_REQUEST enum value
    buffer.append(CONNECT_REQUEST_TYPE)
    buffer.extend(struct.pack('<I', player_id))
    
    name_bytes = player_name.encode('utf-8')
    buffer.extend(struct.pack('<I', len(name_bytes)))
    buffer.extend(name_bytes)
    return buffer


def serialize_chat_message(sender: str, message: str) -> bytearray:
    buffer = bytearray()
    CHAT_MESSAGE_TYPE = 40  # Matches C++ enum CHAT_MESSAGE = 40
    buffer.append(CHAT_MESSAGE_TYPE)
    
    # 1. Sender string (4 bytes length + string bytes)
    sender_bytes = sender.encode('utf-8')
    buffer.extend(struct.pack('<I', len(sender_bytes)))
    buffer.extend(sender_bytes)
    
    # 2. Message string (4 bytes length + string bytes)
    msg_bytes = message.encode('utf-8')
    buffer.extend(struct.pack('<I', len(msg_bytes)))
    buffer.extend(msg_bytes)
    
    return buffer


def receive_loop(sock, encryptor):
    """Continuously listens for, decrypts, and prints incoming server packets (history and broadcasts)"""
    while True:
        try:
            size_buf = sock.recv(4)
            if not size_buf or len(size_buf) < 4:
                sys.stdout.write("\n[Connection closed by server]\n")
                sys.stdout.flush()
                break
                
            msg_size = struct.unpack('<I', size_buf)[0]
            data_buf = sock.recv(msg_size)
            if not data_buf:
                break
                
            payload = bytearray(data_buf)
            encryptor.decrypt(payload)
            
            if len(payload) > 0:
                msg_type = payload[0]
                
                # Check if it's a ChatMessage (type 40)
                if msg_type == 40 and len(payload) > 5:
                    try:
                        offset = 1
                        sender_len = struct.unpack('<I', payload[offset:offset+4])[0]
                        offset += 4
                        
                        if offset + sender_len + 4 <= len(payload):
                            sender = payload[offset:offset+sender_len].decode('utf-8', errors='ignore')
                            offset += sender_len
                            
                            msg_len = struct.unpack('<I', payload[offset:offset+4])[0]
                            offset += 4
                            
                            if offset + msg_len <= len(payload):
                                message = payload[offset:offset+msg_len].decode('utf-8', errors='ignore')
                                
                                # Print chat message cleanly and restore input prompt
                                sys.stdout.write(f"\r\033[K[CHAT] {sender}: {message}\n")
                                sys.stdout.write("Enter message: ")
                                sys.stdout.flush()
                                continue
                    except Exception:
                        pass
                
                # Fallback for system packets (like ConnectResponse)
                sys.stdout.write(f"\r\033[K[System Packet Type {msg_type} Received]\n")
                sys.stdout.write("Enter message: ")
                sys.stdout.flush()
                
        except Exception:
            break


def run_client():
    # Get player name from command line argument, default to "PythonTester" if omitted
    player_name = sys.argv[1] if len(sys.argv) > 1 else "PythonTester"
    
    print(f"Connecting to CloneMine Chat Server at {HOST}:{PORT} as '{player_name}'...")
    
    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect((HOST, PORT))
        print("Connected successfully!")
        
        encryptor = PacketEncryption(SECRET_KEY)
        
        # 1. Send Connection Request Handshake
        connect_payload = serialize_connect_request(player_id=123, player_name=player_name)
        encryptor.encrypt(connect_payload)
        sock.sendall(struct.pack('<I', len(connect_payload)) + connect_payload)
        
        # 2. Start background listener thread immediately
        listener = threading.Thread(target=receive_loop, args=(sock, encryptor), daemon=True)
        listener.start()
        
        print(f"\nLogged in as {player_name}. Type your messages below. Type 'exit' to quit.\n")
        
        # 3. Interactive loop to send chat messages
        while True:
            message = input("Enter message: ")
            if not message:
                continue
            if message.lower() == 'exit':
                break
            
            chat_payload = serialize_chat_message(sender=player_name, message=message)
            encryptor.encrypt(chat_payload)
            
            full_packet = struct.pack('<I', len(chat_payload)) + chat_payload
            sock.sendall(full_packet)
            
    except ConnectionRefusedError:
        print("Connection refused. Is CloneMineChatServer running?")
    except KeyboardInterrupt:
        print("\nExiting client...")
    finally:
        sock.close()

if __name__ == "__main__":
    run_client()
```