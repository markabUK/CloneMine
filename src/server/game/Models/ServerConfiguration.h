#pragma once

#include <string>

namespace CloneMine {
namespace Game {
namespace Models {

struct ServerConfiguration {
    int Port;
    std::string EncryptionKey;
    int MaxPlayers;
    int TickRate;  // Game loop frequency in Hz (e.g., 60)
    double MaxCoordinateValue;
    bool EnableEncryption;
    
    ServerConfiguration()
        : Port(25565),
          EncryptionKey("YourSecretKeyHere1234567890!@#$"),
          MaxPlayers(100),
          TickRate(60),
          MaxCoordinateValue(10000.0),
          EnableEncryption(true) {
    }
    
    ServerConfiguration(int port, const std::string& encryptionKey, 
                       int maxPlayers = 100, int tickRate = 60)
        : Port(port),
          EncryptionKey(encryptionKey),
          MaxPlayers(maxPlayers),
          TickRate(tickRate),
          MaxCoordinateValue(10000.0),
          EnableEncryption(true) {
    }
    
    int GetTickIntervalMs() const {
        return 1000 / TickRate;  // Convert Hz to milliseconds
    }
};

} // namespace Models
} // namespace Game
} // namespace CloneMine
