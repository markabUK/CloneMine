#pragma once

#include "../Interfaces/IEncryptionService.h"
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/sha.h>
#include <vector>
#include <string>
#include <cstring>

namespace CloneMine {
namespace Chat {

/**
 * @brief AES-256-CBC encryption service using OpenSSL
 * 
 * Follows Single Responsibility Principle - only handles encryption/decryption
 * Follows Dependency Inversion Principle - implements IEncryptionService interface
 * Compatible with .NET AES-256-CBC implementation
 */
class AesEncryptionService : public IEncryptionService {
private:
    unsigned char key[32]; // 256-bit key
    
    std::vector<unsigned char> DeriveKey(const std::string& password) {
        std::vector<unsigned char> hash(32);
        SHA256(reinterpret_cast<const unsigned char*>(password.c_str()), 
               password.length(), hash.data());
        return hash;
    }

public:
    AesEncryptionService() {
        // Initialize with default key
        std::memset(key, 0, sizeof(key));
    }

    explicit AesEncryptionService(const std::string& password) {
        SetKey(password);
    }

    void SetKey(const std::string& password) override {
        auto derivedKey = DeriveKey(password);
        std::memcpy(key, derivedKey.data(), 32);
    }

    std::vector<unsigned char> Encrypt(const std::string& plaintext) override {
        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
        if (!ctx) return {};

        // Generate random IV
        unsigned char iv[16];
        RAND_bytes(iv, 16);

        std::vector<unsigned char> ciphertext;
        ciphertext.reserve(16 + plaintext.length() + 16); // IV + data + padding

        // Add IV to the beginning
        ciphertext.insert(ciphertext.end(), iv, iv + 16);

        if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key, iv) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            return {};
        }

        int len;
        unsigned char buffer[1024];
        const unsigned char* input = reinterpret_cast<const unsigned char*>(plaintext.c_str());
        int inputLen = static_cast<int>(plaintext.length());

        if (EVP_EncryptUpdate(ctx, buffer, &len, input, inputLen) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            return {};
        }
        ciphertext.insert(ciphertext.end(), buffer, buffer + len);

        if (EVP_EncryptFinal_ex(ctx, buffer, &len) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            return {};
        }
        ciphertext.insert(ciphertext.end(), buffer, buffer + len);

        EVP_CIPHER_CTX_free(ctx);
        return ciphertext;
    }

    std::string Decrypt(const std::vector<unsigned char>& ciphertext) override {
        if (ciphertext.size() < 16) return "";

        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
        if (!ctx) return "";

        // Extract IV from the beginning
        const unsigned char* iv = ciphertext.data();
        const unsigned char* data = ciphertext.data() + 16;
        int dataLen = static_cast<int>(ciphertext.size()) - 16;

        if (EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key, iv) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            return "";
        }

        std::vector<unsigned char> plaintext;
        plaintext.resize(dataLen + 16);

        int len;
        if (EVP_DecryptUpdate(ctx, plaintext.data(), &len, data, dataLen) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            return "";
        }
        int plaintextLen = len;

        if (EVP_DecryptFinal_ex(ctx, plaintext.data() + len, &len) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            return "";
        }
        plaintextLen += len;

        EVP_CIPHER_CTX_free(ctx);
        return std::string(reinterpret_cast<char*>(plaintext.data()), plaintextLen);
    }
};

} // namespace Chat
} // namespace CloneMine
