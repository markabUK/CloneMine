#pragma once

#include "../Interfaces/IEncryptionService.h"
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/sha.h>
#include <vector>
#include <string>
#include <cstring>

namespace CloneMine {
namespace Game {
namespace Security {

class AesEncryptionService : public Interfaces::IEncryptionService {
private:
    std::vector<unsigned char> key;
    bool enabled;

    std::vector<unsigned char> DeriveKey(const std::string& password) {
        std::vector<unsigned char> hash(SHA256_DIGEST_LENGTH);
        SHA256(reinterpret_cast<const unsigned char*>(password.c_str()), 
               password.length(), hash.data());
        return hash;
    }

public:
    AesEncryptionService(const std::string& encryptionKey, bool enable = true)
        : key(DeriveKey(encryptionKey)), enabled(enable) {
    }

    std::vector<unsigned char> Encrypt(const std::vector<unsigned char>& data) override {
        if (!enabled || data.empty()) {
            return data;
        }

        // Generate random IV
        std::vector<unsigned char> iv(16);
        RAND_bytes(iv.data(), 16);

        // Initialize encryption
        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
        EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key.data(), iv.data());

        // Encrypt
        std::vector<unsigned char> ciphertext(data.size() + EVP_CIPHER_block_size(EVP_aes_256_cbc()));
        int len = 0, ciphertext_len = 0;
        
        EVP_EncryptUpdate(ctx, ciphertext.data(), &len, data.data(), data.size());
        ciphertext_len = len;
        
        EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &len);
        ciphertext_len += len;
        
        EVP_CIPHER_CTX_free(ctx);

        // Prepend IV to ciphertext
        std::vector<unsigned char> result;
        result.insert(result.end(), iv.begin(), iv.end());
        result.insert(result.end(), ciphertext.begin(), ciphertext.begin() + ciphertext_len);

        return result;
    }

    std::vector<unsigned char> Decrypt(const std::vector<unsigned char>& data) override {
        if (!enabled || data.size() < 16) {
            return data;
        }

        // Extract IV
        std::vector<unsigned char> iv(data.begin(), data.begin() + 16);
        std::vector<unsigned char> ciphertext(data.begin() + 16, data.end());

        // Initialize decryption
        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
        EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key.data(), iv.data());

        // Decrypt
        std::vector<unsigned char> plaintext(ciphertext.size());
        int len = 0, plaintext_len = 0;
        
        EVP_DecryptUpdate(ctx, plaintext.data(), &len, ciphertext.data(), ciphertext.size());
        plaintext_len = len;
        
        if (EVP_DecryptFinal_ex(ctx, plaintext.data() + len, &len) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            return data;  // Decryption failed, return original
        }
        plaintext_len += len;
        
        EVP_CIPHER_CTX_free(ctx);

        plaintext.resize(plaintext_len);
        return plaintext;
    }

    bool IsEnabled() const override {
        return enabled;
    }
};

} // namespace Security
} // namespace Game
} // namespace CloneMine
