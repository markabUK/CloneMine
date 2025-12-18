# SOLID Architecture & Security Implementation Guide

## Overview

This document describes the SOLID principles architecture and security features implemented in the experimental .NET servers, and provides guidance for applying the same patterns to:
1. The remaining .NET experimental servers
2. The original C++ servers

## SOLID Principles Applied

### Single Responsibility Principle (SRP)
Each class has one and only one reason to change.

**Example from LoginServer:**
- `Account.cs` - Only represents account data
- `PasswordHasher.cs` - Only handles password hashing
- `InputValidator.cs` - Only validates input
- `AesEncryptionService.cs` - Only handles encryption/decryption
- `LoginMessageHandler.cs` - Only routes and processes messages
- `TcpClientHandler.cs` - Only manages client connections
- `AuthenticationService.cs` - Only handles authentication logic
- `InMemoryAccountRepository.cs` - Only stores/retrieves account data

### Open/Closed Principle (OCP)
Open for extension, closed for modification.

**Implementation:**
- All core functionality defined through interfaces (`IAccountRepository`, `IAuthenticationService`, etc.)
- Can switch from `InMemoryAccountRepository` to `DatabaseAccountRepository` without changing other code
- Can replace `AesEncryptionService` with `TlsEncryptionService` without modifying clients

### Liskov Substitution Principle (LSP)
Objects should be replaceable with instances of their subtypes.

**Implementation:**
- Any `IAccountRepository` implementation can be used interchangeably
- Any `IEncryptionService` implementation provides same encryption guarantees
- All implementations honor their interface contracts

### Interface Segregation Principle (ISP)
Many specific interfaces are better than one general-purpose interface.

**Implementation:**
- `IPasswordHasher` - Focused on password operations only
- `IInputValidator` - Focused on validation only
- `IEncryptionService` - Focused on encryption only
- Not one giant `IServerService` interface

### Dependency Inversion Principle (DIP)
Depend on abstractions, not concrete classes.

**Implementation:**
```csharp
// Good: Depends on abstraction
public class AuthenticationService : IAuthenticationService
{
    private readonly IAccountRepository _accountRepository;  // Interface!
    private readonly IPasswordHasher _passwordHasher;        // Interface!
    private readonly IInputValidator _inputValidator;        // Interface!
}

// Bad: Would depend on concrete class
// private readonly InMemoryAccountRepository _accountRepository;
```

## Security Features Implemented

### 1. Encryption (AES-256)

**Purpose:** Protect data in transit from eavesdropping

**Implementation:**
```csharp
public class AesEncryptionService : IEncryptionService
{
    private readonly byte[] _key;  // 256-bit key from SHA256
    private readonly byte[] _iv;   // 128-bit IV
    
    public byte[] Encrypt(byte[] data) { ... }
    public byte[] Decrypt(byte[] data) { ... }
}
```

**Features:**
- AES-256 encryption with CBC mode
- PKCS7 padding
- Key derived from SHA256 hash
- Null checks on all inputs
- Exception handling for corrupted data

### 2. Password Hashing (PBKDF2-SHA256)

**Purpose:** Securely store passwords (never store plain text!)

**Implementation:**
```csharp
public class PasswordHasher : IPasswordHasher
{
    private const int Iterations = 100,000;  // Slow brute-force attempts
    private const int SaltSize = 16;         // Unique salt per password
    private const int HashSize = 32;         // SHA256 output
    
    public string HashPassword(string password)
    {
        byte[] salt = RandomNumberGenerator.GetBytes(SaltSize);
        byte[] hash = Rfc2898DeriveBytes.Pbkdf2(...);
        return Convert.ToBase64String(salt + hash);
    }
}
```

**Features:**
- PBKDF2 with SHA256
- 100,000 iterations (computationally expensive for attackers)
- Random salt per password (prevents rainbow table attacks)
- Constant-time comparison (prevents timing attacks)

### 3. Input Validation

**Purpose:** Prevent injection attacks and invalid data

**Implementation:**
```csharp
public class InputValidator : IInputValidator
{
    public (bool IsValid, string ErrorMessage) ValidateUsername(string? username)
    {
        // Null check
        if (username == null) return (false, "Username cannot be null");
        
        // Empty check
        if (string.IsNullOrWhiteSpace(username)) return (false, "Username cannot be empty");
        
        // Length check
        if (username.Length < 3 || username.Length > 16) return (false, "Invalid length");
        
        // Format check - alphanumeric only
        if (!UsernameRegex.IsMatch(username)) return (false, "Invalid format");
        
        // SQL injection check
        if (SqlInjectionRegex.IsMatch(username)) return (false, "Invalid characters");
        
        return (true, string.Empty);
    }
}
```

**Protection Against:**
- Null/empty inputs
- SQL injection (`' OR '1'='1`)
- Buffer overflow (length limits)
- Format violations
- Special characters
- Null bytes

## File Structure (SOLID Organization)

```
LoginServer/
├── Program.cs                          # Entry point, dependency injection
├── Models/                             # Data models (SRP)
│   ├── Account.cs
│   ├── LoginResult.cs
│   └── ServerConfiguration.cs
├── Interfaces/                         # Contracts (DIP, ISP)
│   ├── IAccountRepository.cs
│   ├── IAuthenticationService.cs
│   ├── IClientHandler.cs
│   ├── IEncryptionService.cs
│   ├── IInputValidator.cs
│   ├── IMessageHandler.cs
│   └── IPasswordHasher.cs
├── Repositories/                       # Data access (SRP, OCP)
│   └── InMemoryAccountRepository.cs
├── Services/                           # Business logic (SRP)
│   ├── AuthenticationService.cs
│   └── TcpServerListener.cs
├── Handlers/                           # Request processing (SRP)
│   ├── LoginMessageHandler.cs
│   └── TcpClientHandler.cs
├── Security/                           # Security implementations (SRP)
│   ├── AesEncryptionService.cs
│   └── PasswordHasher.cs
└── Validation/                         # Input validation (SRP)
    └── InputValidator.cs
```

## Applying to C++ Servers

### Current C++ Code Issues

**Example from login_server_main.cpp:**
```cpp
// ❌ Multiple responsibilities in one file
// ❌ No encryption
// ❌ Minimal input validation
// ❌ Plain text password comparison
// ❌ No null checks

std::string ProcessMessage(string message) {
    var parts = message.split('|');
    // Direct password comparison - insecure!
    if (_accounts[username] == password) { ... }
}
```

### Refactored C++ Structure

```
src/server/login/
├── main.cpp                            # Entry point only
├── models/
│   ├── Account.h
│   ├── Account.cpp
│   ├── LoginResult.h
│   └── ServerConfiguration.h
├── interfaces/
│   ├── IAccountRepository.h
│   ├── IAuthenticationService.h
│   ├── IEncryptionService.h
│   ├── IInputValidator.h
│   └── IPasswordHasher.h
├── repositories/
│   ├── InMemoryAccountRepository.h
│   └── InMemoryAccountRepository.cpp
├── services/
│   ├── AuthenticationService.h
│   ├── AuthenticationService.cpp
│   ├── TcpServerListener.h
│   └── TcpServerListener.cpp
├── handlers/
│   ├── LoginMessageHandler.h
│   ├── LoginMessageHandler.cpp
│   ├── TcpClientHandler.h
│   └── TcpClientHandler.cpp
├── security/
│   ├── AesEncryptionService.h
│   ├── AesEncryptionService.cpp
│   ├── PasswordHasher.h
│   └── PasswordHasher.cpp
└── validation/
    ├── InputValidator.h
    └── InputValidator.cpp
```

### C++ Implementation Examples

#### 1. Interface (Header File)

```cpp
// IPasswordHasher.h
#pragma once
#include <string>

namespace clonemine::server::login {

class IPasswordHasher {
public:
    virtual ~IPasswordHasher() = default;
    virtual std::string HashPassword(const std::string& password) = 0;
    virtual bool VerifyPassword(const std::string& password, const std::string& hash) = 0;
};

} // namespace
```

#### 2. Implementation with Security

```cpp
// PasswordHasher.cpp
#include "PasswordHasher.h"
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <stdexcept>

namespace clonemine::server::login {

std::string PasswordHasher::HashPassword(const std::string& password) {
    // Null/empty check
    if (password.empty()) {
        throw std::invalid_argument("Password cannot be empty");
    }
    
    // Generate random salt
    unsigned char salt[SALT_SIZE];
    if (RAND_bytes(salt, SALT_SIZE) != 1) {
        throw std::runtime_error("Failed to generate salt");
    }
    
    // Derive key using PBKDF2
    unsigned char hash[HASH_SIZE];
    if (PKCS5_PBKDF2_HMAC(
        password.c_str(), password.length(),
        salt, SALT_SIZE,
        ITERATIONS,
        EVP_sha256(),
        HASH_SIZE,
        hash) != 1) {
        throw std::runtime_error("Failed to hash password");
    }
    
    // Combine salt and hash
    std::vector<unsigned char> combined(SALT_SIZE + HASH_SIZE);
    std::copy(salt, salt + SALT_SIZE, combined.begin());
    std::copy(hash, hash + HASH_SIZE, combined.begin() + SALT_SIZE);
    
    return Base64Encode(combined);
}

bool PasswordHasher::VerifyPassword(const std::string& password, const std::string& hash) {
    // Null/empty checks
    if (password.empty() || hash.empty()) {
        return false;
    }
    
    try {
        // Decode base64
        auto hashBytes = Base64Decode(hash);
        if (hashBytes.size() != SALT_SIZE + HASH_SIZE) {
            return false;
        }
        
        // Extract salt
        std::vector<unsigned char> salt(hashBytes.begin(), 
                                       hashBytes.begin() + SALT_SIZE);
        
        // Recompute hash
        unsigned char computed[HASH_SIZE];
        if (PKCS5_PBKDF2_HMAC(
            password.c_str(), password.length(),
            salt.data(), SALT_SIZE,
            ITERATIONS,
            EVP_sha256(),
            HASH_SIZE,
            computed) != 1) {
            return false;
        }
        
        // Constant-time comparison
        int diff = 0;
        for (int i = 0; i < HASH_SIZE; i++) {
            diff |= hashBytes[SALT_SIZE + i] ^ computed[i];
        }
        
        return diff == 0;
    } catch (...) {
        return false;
    }
}

} // namespace
```

#### 3. Input Validation with Null Checks

```cpp
// InputValidator.cpp
#include "InputValidator.h"
#include <regex>
#include <algorithm>

namespace clonemine::server::login {

ValidationResult InputValidator::ValidateUsername(const std::string* username) {
    // Null check
    if (username == nullptr) {
        return {false, "Username cannot be null"};
    }
    
    // Empty check
    if (username->empty()) {
        return {false, "Username cannot be empty"};
    }
    
    // Length check
    if (username->length() < config_.minUsernameLength || 
        username->length() > config_.maxUsernameLength) {
        return {false, "Username length is invalid"};
    }
    
    // Check for null bytes
    if (username->find('\0') != std::string::npos) {
        return {false, "Username contains null bytes"};
    }
    
    // Format validation - alphanumeric and underscore only
    static const std::regex usernameRegex("^[a-zA-Z0-9_]+$");
    if (!std::regex_match(*username, usernameRegex)) {
        return {false, "Username contains invalid characters"};
    }
    
    // SQL injection check
    static const std::regex sqlInjectionRegex(
        R"(('|(;|--)|(\bOR\b)|(\bAND\b)|(\bUNION\b)|(\bSELECT\b)))",
        std::regex::icase);
    if (std::regex_search(*username, sqlInjectionRegex)) {
        return {false, "Username contains forbidden patterns"};
    }
    
    return {true, ""};
}

ValidationResult InputValidator::ValidatePassword(const std::string* password) {
    // Null check
    if (password == nullptr) {
        return {false, "Password cannot be null"};
    }
    
    // Empty check
    if (password->empty()) {
        return {false, "Password cannot be empty"};
    }
    
    // Length check
    if (password->length() < config_.minPasswordLength || 
        password->length() > config_.maxPasswordLength) {
        return {false, "Password length is invalid"};
    }
    
    // Check for null bytes
    if (password->find('\0') != std::string::npos) {
        return {false, "Password contains null bytes"};
    }
    
    return {true, ""};
}

} // namespace
```

#### 4. Encryption Service

```cpp
// AesEncryptionService.cpp
#include "AesEncryptionService.h"
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/sha.h>
#include <stdexcept>

namespace clonemine::server::login {

AesEncryptionService::AesEncryptionService(const std::string& key) {
    // Generate 256-bit key from provided string
    unsigned char keyHash[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(key.c_str()), 
           key.length(), keyHash);
    key_.assign(keyHash, keyHash + 32);
    
    // Generate IV
    std::string ivSource = key + "IV";
    unsigned char ivHash[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(ivSource.c_str()), 
           ivSource.length(), ivHash);
    iv_.assign(ivHash, ivHash + 16);  // AES block size
}

std::vector<unsigned char> AesEncryptionService::Encrypt(
    const std::vector<unsigned char>& data) {
    
    // Null/empty check
    if (data.empty()) {
        throw std::invalid_argument("Data cannot be empty");
    }
    
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        throw std::runtime_error("Failed to create cipher context");
    }
    
    std::vector<unsigned char> ciphertext(data.size() + AES_BLOCK_SIZE);
    int len = 0;
    int ciphertext_len = 0;
    
    try {
        // Initialize encryption
        if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, 
                              key_.data(), iv_.data()) != 1) {
            throw std::runtime_error("Failed to initialize encryption");
        }
        
        // Encrypt data
        if (EVP_EncryptUpdate(ctx, ciphertext.data(), &len, 
                             data.data(), data.size()) != 1) {
            throw std::runtime_error("Failed to encrypt data");
        }
        ciphertext_len = len;
        
        // Finalize encryption
        if (EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &len) != 1) {
            throw std::runtime_error("Failed to finalize encryption");
        }
        ciphertext_len += len;
        
        ciphertext.resize(ciphertext_len);
        EVP_CIPHER_CTX_free(ctx);
        
        return ciphertext;
    } catch (...) {
        EVP_CIPHER_CTX_free(ctx);
        throw;
    }
}

std::vector<unsigned char> AesEncryptionService::Decrypt(
    const std::vector<unsigned char>& data) {
    
    // Null/empty check
    if (data.empty()) {
        throw std::invalid_argument("Data cannot be empty");
    }
    
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        throw std::runtime_error("Failed to create cipher context");
    }
    
    std::vector<unsigned char> plaintext(data.size());
    int len = 0;
    int plaintext_len = 0;
    
    try {
        // Initialize decryption
        if (EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, 
                              key_.data(), iv_.data()) != 1) {
            throw std::runtime_error("Failed to initialize decryption");
        }
        
        // Decrypt data
        if (EVP_DecryptUpdate(ctx, plaintext.data(), &len, 
                             data.data(), data.size()) != 1) {
            throw std::runtime_error("Failed to decrypt data");
        }
        plaintext_len = len;
        
        // Finalize decryption
        if (EVP_DecryptFinal_ex(ctx, plaintext.data() + len, &len) != 1) {
            throw std::runtime_error("Invalid encryption or corrupted data");
        }
        plaintext_len += len;
        
        plaintext.resize(plaintext_len);
        EVP_CIPHER_CTX_free(ctx);
        
        return plaintext;
    } catch (...) {
        EVP_CIPHER_CTX_free(ctx);
        throw;
    }
}

} // namespace
```

### Dependency Injection in C++

```cpp
// main.cpp
#include "interfaces/IAccountRepository.h"
#include "interfaces/IPasswordHasher.h"
#include "interfaces/IInputValidator.h"
#include "interfaces/IEncryptionService.h"
#include "interfaces/IAuthenticationService.h"
#include "repositories/InMemoryAccountRepository.h"
#include "security/PasswordHasher.h"
#include "security/AesEncryptionService.h"
#include "validation/InputValidator.h"
#include "services/AuthenticationService.h"
#include "handlers/LoginMessageHandler.h"
#include "handlers/TcpClientHandler.h"
#include "services/TcpServerListener.h"

int main(int argc, char* argv[]) {
    try {
        // Configuration
        ServerConfiguration config;
        // ... parse command line args
        
        // Create dependencies (manual dependency injection)
        auto accountRepo = std::make_unique<InMemoryAccountRepository>();
        auto passwordHasher = std::make_unique<PasswordHasher>();
        auto inputValidator = std::make_unique<InputValidator>(config);
        auto encryptionService = std::make_unique<AesEncryptionService>("SecretKey");
        
        auto authService = std::make_unique<AuthenticationService>(
            accountRepo.get(),
            passwordHasher.get(),
            inputValidator.get());
        
        auto messageHandler = std::make_unique<LoginMessageHandler>(
            authService.get(),
            inputValidator.get(),
            encryptionService.get(),
            config);
        
        auto clientHandler = std::make_unique<TcpClientHandler>(
            messageHandler.get(),
            encryptionService.get());
        
        auto server = std::make_unique<TcpServerListener>(
            config,
            clientHandler.get());
        
        // Start server
        server->Start(config.port);
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
}
```

## Required Libraries for C++

### OpenSSL for Encryption/Hashing
```bash
# Ubuntu/Debian
sudo apt-get install libssl-dev

# macOS
brew install openssl

# Windows (vcpkg)
vcpkg install openssl
```

### CMakeLists.txt Updates
```cmake
find_package(OpenSSL REQUIRED)

target_link_libraries(clonemine_login_server
    PRIVATE
        OpenSSL::SSL
        OpenSSL::Crypto
)
```

## Security Checklist

### For All Servers (C++ and C#)

- [ ] **Encryption**
  - [ ] All network traffic encrypted (AES-256 or TLS)
  - [ ] Encryption keys properly managed
  - [ ] IV/nonce properly generated

- [ ] **Password Security**
  - [ ] Never store plain text passwords
  - [ ] Use PBKDF2, bcrypt, or Argon2
  - [ ] Minimum 100,000 iterations
  - [ ] Unique salt per password
  - [ ] Constant-time comparison

- [ ] **Input Validation**
  - [ ] Null checks on all inputs
  - [ ] Empty string checks
  - [ ] Length validation
  - [ ] Format validation (regex)
  - [ ] SQL injection protection
  - [ ] XSS protection
  - [ ] Null byte checks
  - [ ] Buffer overflow protection

- [ ] **Error Handling**
  - [ ] Never expose internal errors to clients
  - [ ] Log detailed errors server-side
  - [ ] Return generic errors to clients
  - [ ] No stack traces in responses

- [ ] **Rate Limiting**
  - [ ] Limit login attempts
  - [ ] Limit message frequency
  - [ ] Prevent DoS attacks

- [ ] **Logging**
  - [ ] Log all authentication attempts
  - [ ] Log suspicious activity
  - [ ] Never log passwords
  - [ ] Log encryption failures

## Migration Strategy

### Phase 1: Add Interfaces (C++)
1. Create interface headers for each responsibility
2. No implementation changes yet
3. Build should still work

### Phase 2: Extract Classes (C++)
1. Move logic from monolithic files into separate classes
2. Implement interfaces
3. Test each class independently

### Phase 3: Add Security (C++)
1. Add encryption service
2. Add password hasher
3. Add input validator
4. Test security features

### Phase 4: Integration (C++)
1. Wire up dependencies in main.cpp
2. Replace old code with new implementations
3. Test full system

### Phase 5: Remaining .NET Servers
1. Apply same pattern to GameServer
2. Apply to ChatServer
3. Apply to QuestServer
4. Apply to CharacterServer
5. Apply to AuctionServer

## Testing Security Features

### Test Encryption
```cpp
auto enc = std::make_unique<AesEncryptionService>("TestKey");
std::vector<unsigned char> data = {'H', 'e', 'l', 'l', 'o'};
auto encrypted = enc->Encrypt(data);
auto decrypted = enc->Decrypt(encrypted);
assert(data == decrypted);
```

### Test Password Hashing
```cpp
auto hasher = std::make_unique<PasswordHasher>();
auto hash = hasher->HashPassword("password123");
assert(hasher->VerifyPassword("password123", hash) == true);
assert(hasher->VerifyPassword("wrongpass", hash) == false);
```

### Test Input Validation
```cpp
auto validator = std::make_unique<InputValidator>(config);
auto result1 = validator->ValidateUsername("valid_user");
assert(result1.isValid == true);

auto result2 = validator->ValidateUsername("' OR '1'='1");
assert(result2.isValid == false);

auto result3 = validator->ValidateUsername(nullptr);
assert(result3.isValid == false);
```

## Performance Considerations

### Encryption Overhead
- AES-256-CBC: ~50-100MB/s (acceptable for game servers)
- Consider caching encrypted common messages
- Use connection pooling to amortize handshake costs

### Hashing Overhead
- PBKDF2 with 100K iterations: ~50-100ms per hash
- Only hash on registration/password change
- Use session tokens after login

### Validation Overhead
- Regex validation: <1ms per input
- Negligible impact on throughput
- Essential for security

## Production Recommendations

1. **Use TLS/SSL** instead of custom encryption
2. **Use a proper key management system** (not hardcoded keys)
3. **Implement rate limiting** (e.g., max 5 login attempts per minute)
4. **Add proper logging** with log rotation
5. **Use a real database** instead of in-memory storage
6. **Implement session management** with tokens (JWT)
7. **Add monitoring** and alerting
8. **Regular security audits**
9. **Keep dependencies updated**
10. **Use secrets management** (AWS Secrets Manager, Azure Key Vault, etc.)

## Conclusion

This architecture provides:
- ✅ Clean, maintainable code (SOLID principles)
- ✅ Testable components (dependency injection)
- ✅ Secure communication (encryption)
- ✅ Secure storage (password hashing)
- ✅ Protected against common attacks (validation)
- ✅ Easy to extend (open/closed principle)
- ✅ Cross-platform compatibility

Apply these patterns to all servers for a robust, secure game server infrastructure.
