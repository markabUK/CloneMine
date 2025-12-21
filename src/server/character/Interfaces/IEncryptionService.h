#pragma once
#include <string>
#include <vector>

class IEncryptionService {
public:
    virtual ~IEncryptionService() = default;
    
    virtual std::vector<unsigned char> Encrypt(const std::string& plaintext) = 0;
    virtual std::string Decrypt(const std::vector<unsigned char>& ciphertext) = 0;
};
