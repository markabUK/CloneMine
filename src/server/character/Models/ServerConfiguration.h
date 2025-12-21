#pragma once
#include <string>

struct ServerConfiguration {
    int port;
    std::string encryptionKey;
    bool enableEncryption;
    int maxClients;
    
    ServerConfiguration()
        : port(25568), encryptionKey("clonemine-character-key-2024"), 
          enableEncryption(true), maxClients(100) {}
};
