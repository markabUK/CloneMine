#pragma once

#include "../Interfaces/IPasswordHasher.h"
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstring>

namespace CloneMine {
namespace Common {
namespace Security {

/**
 * @brief PBKDF2-SHA256 password hasher using OpenSSL
 * 
 * Compatible with .NET PasswordHasher for interoperability.
 * 
 * Features:
 * - PBKDF2-SHA256 with 100,000 iterations
 * - 16-byte random salt generation
 * - 32-byte hash output
 * - Constant-time password comparison (timing attack prevention)
 * - Hash format: "iterations.salt.hash" (hex-encoded)
 * 
 * Shared implementation used by all C++ servers.
 */
class PasswordHasher : public IPasswordHasher {
private:
    static constexpr int SALT_SIZE = 16;
    static constexpr int HASH_SIZE = 32;
    static constexpr int ITERATIONS = 100000;

    std::vector<unsigned char> GenerateSalt() {
        std::vector<unsigned char> salt(SALT_SIZE);
        if (RAND_bytes(salt.data(), SALT_SIZE) != 1) {
            throw std::runtime_error("Failed to generate random salt");
        }
        return salt;
    }

    std::string ToHex(const std::vector<unsigned char>& data) {
        std::ostringstream oss;
        for (unsigned char byte : data) {
            oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
        }
        return oss.str();
    }

    std::vector<unsigned char> FromHex(const std::string& hex) {
        std::vector<unsigned char> bytes;
        for (size_t i = 0; i < hex.length(); i += 2) {
            std::string byteString = hex.substr(i, 2);
            unsigned char byte = static_cast<unsigned char>(std::stoi(byteString, nullptr, 16));
            bytes.push_back(byte);
        }
        return bytes;
    }

    std::vector<unsigned char> HashPasswordWithSalt(const std::string& password, 
                                                      const std::vector<unsigned char>& salt,
                                                      int iterations) {
        std::vector<unsigned char> hash(HASH_SIZE);
        
        if (PKCS5_PBKDF2_HMAC(password.c_str(), password.length(),
                              salt.data(), salt.size(),
                              iterations,
                              EVP_sha256(),
                              HASH_SIZE, hash.data()) != 1) {
            throw std::runtime_error("PBKDF2 hashing failed");
        }
        
        return hash;
    }

    bool ConstantTimeEquals(const std::vector<unsigned char>& a, 
                           const std::vector<unsigned char>& b) {
        if (a.size() != b.size()) {
            return false;
        }
        
        unsigned char result = 0;
        for (size_t i = 0; i < a.size(); i++) {
            result |= a[i] ^ b[i];
        }
        
        return result == 0;
    }

public:
    std::string HashPassword(const std::string& password) override {
        if (password.empty()) {
            throw std::invalid_argument("Password cannot be empty");
        }

        auto salt = GenerateSalt();
        auto hash = HashPasswordWithSalt(password, salt, ITERATIONS);

        // Format: iterations.salt.hash (all hex-encoded)
        std::ostringstream oss;
        oss << ITERATIONS << "." << ToHex(salt) << "." << ToHex(hash);
        return oss.str();
    }

    bool VerifyPassword(const std::string& password, const std::string& hashedPassword) override {
        if (password.empty() || hashedPassword.empty()) {
            return false;
        }

        try {
            // Parse format: iterations.salt.hash
            size_t firstDot = hashedPassword.find('.');
            size_t secondDot = hashedPassword.find('.', firstDot + 1);
            
            if (firstDot == std::string::npos || secondDot == std::string::npos) {
                return false;
            }

            std::string iterationsStr = hashedPassword.substr(0, firstDot);
            std::string saltHex = hashedPassword.substr(firstDot + 1, secondDot - firstDot - 1);
            std::string hashHex = hashedPassword.substr(secondDot + 1);

            int iterations = std::stoi(iterationsStr);
            if (iterations <= 0) {
                return false;
            }

            auto salt = FromHex(saltHex);
            auto storedHash = FromHex(hashHex);
            auto computedHash = HashPasswordWithSalt(password, salt, iterations);

            // Constant-time comparison to prevent timing attacks
            return ConstantTimeEquals(computedHash, storedHash);
        }
        catch (...) {
            return false;
        }
    }
};

} // namespace Security
} // namespace Common
} // namespace CloneMine
