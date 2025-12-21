# C++ Server Refactoring - SOLID Architecture Implementation

## Overview

This document provides the complete plan for refactoring all 6 C++ servers to match the .NET SOLID architecture with full interoperability.

## Status

**Started:** LoginServer Models and Interfaces (11 files created)
**Remaining:** Repositories, Services, Handlers, Security, Validation for all 6 servers

## Architecture Pattern (Matching .NET)

Each server follows this structure:

```
src/server/{servername}/
├── Models/              # Data classes (Account, Player, Quest, etc.)
├── Interfaces/          # 6-7 abstract interfaces
├── Repositories/        # Data access implementations
├── Services/            # Business logic
├── Handlers/            # Message/client handling
├── Security/            # OpenSSL encryption + PBKDF2
├── Validation/          # Input validation
└── {ServerName}.cpp/h   # Main server class (DI composition)
```

## Files Created So Far

### LoginServer - Models (4 files)
- ✅ `Models/Account.h` - User account data
- ✅ `Models/LoginSession.h` - Active session state
- ✅ `Models/LoginResult.h` - Login operation result
- ✅ `Models/ServerConfiguration.h` - Server settings

### LoginServer - Interfaces (7 files)
- ✅ `Interfaces/IAccountRepository.h` - Account data access
- ✅ `Interfaces/IAuthenticationService.h` - Authentication operations
- ✅ `Interfaces/IPasswordHasher.h` - Password hashing
- ✅ `Interfaces/IInputValidator.h` - Input validation
- ✅ `Interfaces/IEncryptionService.h` - AES-256 encryption
- ✅ `Interfaces/IMessageHandler.h` - Message processing
- ✅ `Interfaces/IClientHandler.h` - Client lifecycle management

## Remaining Work

### 1. LoginServer Completion (9 files)

**Repositories/** (1 file)
- `InMemoryAccountRepository.h/cpp` - Implements IAccountRepository

**Services/** (2 files)
- `AuthenticationService.h/cpp` - Implements IAuthenticationService
- `TcpServerListener.h/cpp` - TCP server management

**Handlers/** (2 files)
- `LoginMessageHandler.h/cpp` - Implements IMessageHandler
- `TcpClientHandler.h/cpp` - Implements IClientHandler

**Security/** (2 files)
- `AesEncryptionService.h/cpp` - Implements IEncryptionService using OpenSSL
- `PasswordHasher.h/cpp` - Implements IPasswordHasher with PBKDF2

**Validation/** (1 file)
- `InputValidator.h/cpp` - Implements IInputValidator

**Main Server** (1 file)
- Refactor `LoginServer.cpp/h` to use dependency injection

### 2. GameServer (Same pattern, ~18 files)

**Models/**
- Player.h
- PlayerState.h
- ServerConfiguration.h

**Interfaces/** (6)
- IPlayerRepository.h
- IGameService.h
- IInputValidator.h
- IEncryptionService.h
- IMessageHandler.h
- IClientHandler.h

**+ Repositories, Services, Handlers, Security, Validation**

### 3. ChatServer (~16 files)

**Models/**
- ChatMessage.h
- ChatClient.h
- ServerConfiguration.h

**+ Full SOLID structure**

### 4. QuestServer (~20 files)

**Models/**
- Quest.h
- QuestType.h
- PlayerQuestProgress.h
- QuestClient.h
- ServerConfiguration.h

**+ Full SOLID structure**

### 5. CharacterServer (~18 files)

**Models/**
- Character.h (reuse from character/CharacterData.h)
- CharacterSession.h
- ServerConfiguration.h

**+ Full SOLID structure**

### 6. AuctionServer (~17 files - NEW)

Currently no C++ AuctionServer exists. Need to create from scratch:

**Models/**
- AuctionItem.h
- Bid.h
- AuctionClient.h
- ServerConfiguration.h

**+ Full SOLID structure**

## OpenSSL Integration

All servers need OpenSSL for encryption and hashing:

### Required OpenSSL Components

```cpp
// AES-256-CBC encryption
#include <openssl/evp.h>
#include <openssl/aes.h>

// PBKDF2-SHA256 for password hashing
#include <openssl/sha.h>
#include <openssl/evp.h>

// Random number generation
#include <openssl/rand.h>
```

### CMakeLists.txt Updates

Need to add OpenSSL dependency:

```cmake
find_package(OpenSSL REQUIRED)

# For each server executable
target_link_libraries(login_server PRIVATE OpenSSL::SSL OpenSSL::Crypto)
target_link_libraries(game_server PRIVATE OpenSSL::SSL OpenSSL::Crypto)
# ... etc for all servers
```

## Protocol Compatibility

To ensure C++ and .NET servers are interchangeable:

### 1. Message Format (Both implementations)
```
[4 bytes: message length][encrypted payload]
```

### 2. Encryption
- AES-256-CBC with same shared key
- Same IV generation method
- Try encrypted first, fallback to plaintext

### 3. Password Hashing (LoginServer only)
- PBKDF2-SHA256
- 100,000 iterations
- 16-byte salt
- 32-byte output

### 4. Session Tokens
- 128-bit hex string (32 characters)
- Same generation algorithm

### 5. Validation Rules
- Username: 3-16 chars, alphanumeric + underscore
- Password: 8-64 chars
- SQL injection regex patterns
- Null byte detection

## Implementation Priority

1. **LoginServer** (Template) - Complete all layers
2. **Copy Security classes** to other servers (AesEncryptionService, PasswordHasher reusable)
3. **Copy Validation** base class (customized per server)
4. **GameServer** - High priority (main gameplay)
5. **ChatServer** - Medium priority
6. **CharacterServer** - Medium priority
7. **QuestServer** - Lower priority
8. **AuctionServer** - Create new (lowest priority)

## Testing Strategy

After each server refactoring:

1. **Build test**: `cmake --build . --target {server_name}`
2. **Protocol test**: Verify C++ server can communicate with .NET client
3. **Interoperability test**: Swap C++ server for .NET server without client changes
4. **Security test**: Verify encryption/decryption works with .NET implementation
5. **Validation test**: Confirm same inputs rejected by both implementations

## Estimated File Counts

- LoginServer: 19 files (11 done, 8 remaining)
- GameServer: 18 files
- ChatServer: 16 files
- QuestServer: 20 files
- CharacterServer: 18 files
- AuctionServer: 17 files (new)

**Total: ~108 C++ files** (matching 104+ .NET files)

## Next Steps

1. Complete LoginServer implementation (8 remaining files)
2. Build and test LoginServer
3. Use as template for remaining 5 servers
4. Update CMakeLists.txt for all servers
5. Add OpenSSL dependencies
6. Test interoperability between C++ and .NET

## Code Style

Match existing CloneMine C++ conventions:
- PascalCase for classes
- camelCase for methods
- m_ prefix for member variables
- Doxygen-style comments
- #pragma once for headers
- clonemine::server::{servername} namespace

## Compatibility Matrix

| Feature | .NET Implementation | C++ Implementation |
|---------|--------------------|--------------------|
| SOLID Architecture | ✅ All 6 servers | ⚠️ In progress (1/6) |
| AES-256 Encryption | ✅ All servers | ⏳ Planned (OpenSSL) |
| PBKDF2 Hashing | ✅ LoginServer | ⏳ Planned (OpenSSL) |
| Input Validation | ✅ All servers | ⏳ Planned |
| Null Checks | ✅ All servers | ⏳ Planned |
| SQL Injection Protection | ✅ All servers | ⏳ Planned |
| Protocol Compatible | ✅ | ⏳ Needs verification |
| Interchangeable | ❌ | ⏳ Goal after completion |
