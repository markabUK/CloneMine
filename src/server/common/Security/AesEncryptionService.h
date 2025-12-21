#pragma once

#include "../Interfaces/IEncryptionService.h"
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <openssl/rand.h>
#include <vector>
#include <string>
#include <cstring>

namespace CloneMine {
namespace Common {
namespace Security {

/**
 * @brief AES-256-CBC encryption service using OpenSSL
 * 
 * Implements IEncryptionService using AES-256-CBC encryption.
 * Compatible with .NET AesEncryptionService for interoperability.
 * 
 * Features:
 * - AES-256-CBC encryption
 * - SHA-256 key derivation
 * - Random IV generation per message
 * - PKCS7 padding
 * 
 * Shared implementation used by all C++ servers.
 */
class AesEncryptionService : public IEncryptionService {
private:
    std::vector<unsigned char> key_;
    bool initialized_;

    std::vector<unsigned char> DeriveKey(const std::string& password) {
        std::vector<unsigned char> hash(SHA256_DIGEST_LENGTH);
        SHA256(reinterpret_cast<const unsigned char*>(password.c_str()), 
               password.length(), 
               hash.data());
        return hash;
    }

    std::vector<unsigned char> GenerateIV() {
        std::vector<unsigned char> iv(AES_BLOCK_SIZE);
        if (RAND_bytes(iv.data(), AES_BLOCK_SIZE) != 1) {
            throw std::runtime_error("Failed to generate random IV");
        }
        return iv;
    }

public:
    explicit AesEncryptionService(const std::string& password) 
        : key_(DeriveKey(password)), initialized_(true) {
    }

    std::vector<unsigned char> Encrypt(const std::vector<unsigned char>& plaintext) override {
        if (!initialized_) {
            throw std::runtime_error("Encryption service not initialized");
        }

        auto iv = GenerateIV();
        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
        if (!ctx) {
            throw std::runtime_error("Failed to create cipher context");
        }

        try {
            if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key_.data(), iv.data()) != 1) {
                throw std::runtime_error("Failed to initialize encryption");
            }

            std::vector<unsigned char> ciphertext(plaintext.size() + AES_BLOCK_SIZE);
            int len = 0;
            int ciphertext_len = 0;

            if (EVP_EncryptUpdate(ctx, ciphertext.data(), &len, plaintext.data(), plaintext.size()) != 1) {
                throw std::runtime_error("Encryption failed");
            }
            ciphertext_len = len;

            if (EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &len) != 1) {
                throw std::runtime_error("Encryption finalization failed");
            }
            ciphertext_len += len;

            ciphertext.resize(ciphertext_len);

            // Prepend IV to ciphertext
            std::vector<unsigned char> result;
            result.reserve(iv.size() + ciphertext.size());
            result.insert(result.end(), iv.begin(), iv.end());
            result.insert(result.end(), ciphertext.begin(), ciphertext.end());

            EVP_CIPHER_CTX_free(ctx);
            return result;
        }
        catch (...) {
            EVP_CIPHER_CTX_free(ctx);
            throw;
        }
    }

    std::vector<unsigned char> Decrypt(const std::vector<unsigned char>& ciphertext) override {
        if (!initialized_) {
            throw std::runtime_error("Encryption service not initialized");
        }

        if (ciphertext.size() < AES_BLOCK_SIZE) {
            throw std::runtime_error("Ciphertext too short (missing IV)");
        }

        // Extract IV from beginning of ciphertext
        std::vector<unsigned char> iv(ciphertext.begin(), ciphertext.begin() + AES_BLOCK_SIZE);
        std::vector<unsigned char> encrypted_data(ciphertext.begin() + AES_BLOCK_SIZE, ciphertext.end());

        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
        if (!ctx) {
            throw std::runtime_error("Failed to create cipher context");
        }

        try {
            if (EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key_.data(), iv.data()) != 1) {
                throw std::runtime_error("Failed to initialize decryption");
            }

            std::vector<unsigned char> plaintext(encrypted_data.size());
            int len = 0;
            int plaintext_len = 0;

            if (EVP_DecryptUpdate(ctx, plaintext.data(), &len, encrypted_data.data(), encrypted_data.size()) != 1) {
                throw std::runtime_error("Decryption failed");
            }
            plaintext_len = len;

            if (EVP_DecryptFinal_ex(ctx, plaintext.data() + len, &len) != 1) {
                throw std::runtime_error("Decryption finalization failed (bad padding or key)");
            }
            plaintext_len += len;

            plaintext.resize(plaintext_len);

            EVP_CIPHER_CTX_free(ctx);
            return plaintext;
        }
        catch (...) {
            EVP_CIPHER_CTX_free(ctx);
            throw;
        }
    }

    std::string EncryptString(const std::string& message) override {
        std::vector<unsigned char> plaintext(message.begin(), message.end());
        auto encrypted = Encrypt(plaintext);
        return std::string(encrypted.begin(), encrypted.end());
    }

    std::string DecryptString(const std::string& encryptedMessage) override {
        std::vector<unsigned char> ciphertext(encryptedMessage.begin(), encryptedMessage.end());
        auto decrypted = Decrypt(ciphertext);
        return std::string(decrypted.begin(), decrypted.end());
    }
};

} // namespace Security
} // namespace Common
} // namespace CloneMine
