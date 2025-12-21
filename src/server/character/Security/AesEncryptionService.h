#pragma once
#include "../Interfaces/IEncryptionService.h"
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/sha.h>
#include <stdexcept>
#include <cstring>

class AesEncryptionService : public IEncryptionService {
private:
    std::vector<unsigned char> key;
    
    void DeriveKey(const std::string& password) {
        key.resize(32); // 256 bits
        SHA256(reinterpret_cast<const unsigned char*>(password.c_str()), 
               password.length(), key.data());
    }
    
public:
    AesEncryptionService(const std::string& password) {
        DeriveKey(password);
    }
    
    std::vector<unsigned char> Encrypt(const std::string& plaintext) override {
        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
        if (!ctx) {
            throw std::runtime_error("Failed to create cipher context");
        }
        
        // Generate random IV
        std::vector<unsigned char> iv(EVP_CIPHER_iv_length(EVP_aes_256_cbc()));
        if (!RAND_bytes(iv.data(), iv.size())) {
            EVP_CIPHER_CTX_free(ctx);
            throw std::runtime_error("Failed to generate IV");
        }
        
        if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key.data(), iv.data()) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            throw std::runtime_error("Failed to initialize encryption");
        }
        
        std::vector<unsigned char> ciphertext(plaintext.length() + EVP_CIPHER_block_size(EVP_aes_256_cbc()));
        int len;
        
        if (EVP_EncryptUpdate(ctx, ciphertext.data(), &len,
                             reinterpret_cast<const unsigned char*>(plaintext.c_str()),
                             plaintext.length()) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            throw std::runtime_error("Failed to encrypt");
        }
        
        int ciphertext_len = len;
        
        if (EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &len) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            throw std::runtime_error("Failed to finalize encryption");
        }
        
        ciphertext_len += len;
        ciphertext.resize(ciphertext_len);
        
        EVP_CIPHER_CTX_free(ctx);
        
        // Prepend IV to ciphertext
        std::vector<unsigned char> result;
        result.insert(result.end(), iv.begin(), iv.end());
        result.insert(result.end(), ciphertext.begin(), ciphertext.end());
        
        return result;
    }
    
    std::string Decrypt(const std::vector<unsigned char>& ciphertext) override {
        if (ciphertext.size() < EVP_CIPHER_iv_length(EVP_aes_256_cbc())) {
            throw std::runtime_error("Ciphertext too short");
        }
        
        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
        if (!ctx) {
            throw std::runtime_error("Failed to create cipher context");
        }
        
        // Extract IV from ciphertext
        size_t iv_len = EVP_CIPHER_iv_length(EVP_aes_256_cbc());
        std::vector<unsigned char> iv(ciphertext.begin(), ciphertext.begin() + iv_len);
        std::vector<unsigned char> encrypted_data(ciphertext.begin() + iv_len, ciphertext.end());
        
        if (EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key.data(), iv.data()) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            throw std::runtime_error("Failed to initialize decryption");
        }
        
        std::vector<unsigned char> plaintext(encrypted_data.size() + EVP_CIPHER_block_size(EVP_aes_256_cbc()));
        int len;
        
        if (EVP_DecryptUpdate(ctx, plaintext.data(), &len, encrypted_data.data(), encrypted_data.size()) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            throw std::runtime_error("Failed to decrypt");
        }
        
        int plaintext_len = len;
        
        if (EVP_DecryptFinal_ex(ctx, plaintext.data() + len, &len) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            throw std::runtime_error("Failed to finalize decryption");
        }
        
        plaintext_len += len;
        plaintext.resize(plaintext_len);
        
        EVP_CIPHER_CTX_free(ctx);
        
        return std::string(plaintext.begin(), plaintext.end());
    }
};
