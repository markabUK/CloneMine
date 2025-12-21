#ifndef QUEST_IENCRYPTION_SERVICE_H
#define QUEST_IENCRYPTION_SERVICE_H

#include <string>

namespace CloneMine {
namespace Quest {

class IEncryptionService {
public:
    virtual ~IEncryptionService() = default;
    
    virtual std::string Encrypt(const std::string& plaintext) = 0;
    virtual std::string Decrypt(const std::string& ciphertext) = 0;
};

} // namespace Quest
} // namespace CloneMine

#endif
