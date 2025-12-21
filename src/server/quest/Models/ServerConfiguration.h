#ifndef QUEST_SERVER_CONFIGURATION_H
#define QUEST_SERVER_CONFIGURATION_H

#include <string>

namespace CloneMine {
namespace Quest {

class ServerConfiguration {
public:
    int port;
    std::string encryptionKey;
    bool useEncryption;
    int maxClients;
    
    ServerConfiguration() 
        : port(25567), 
          encryptionKey("DefaultEncryptionKey123456789012"),
          useEncryption(true),
          maxClients(100) {}
    
    ServerConfiguration(int port, const std::string& encryptionKey, bool useEncryption, int maxClients)
        : port(port), encryptionKey(encryptionKey), useEncryption(useEncryption), maxClients(maxClients) {}
};

} // namespace Quest
} // namespace CloneMine

#endif
