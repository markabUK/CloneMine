#pragma once

#include <string>
#include <vector>

namespace CloneMine {
namespace Game {
namespace Interfaces {

class IEncryptionService {
public:
    virtual ~IEncryptionService() = default;
    
    virtual std::vector<unsigned char> Encrypt(const std::vector<unsigned char>& data) = 0;
    virtual std::vector<unsigned char> Decrypt(const std::vector<unsigned char>& data) = 0;
    virtual bool IsEnabled() const = 0;
};

} // namespace Interfaces
} // namespace Game
} // namespace CloneMine
