#pragma once

#include <string>
#include <cstdint>
#include <fstream>
#include <iostream>

namespace clonemine {
namespace config {

// Configuration for server addresses (supports distributed deployment)
struct ServerConfig {
    // Login Server
    std::string loginServerHost{"localhost"};
    uint16_t loginServerPort{25564};
    
    // Character Server
    std::string characterServerHost{"localhost"};
    uint16_t characterServerPort{25568};
    
    // Game Server
    std::string gameServerHost{"localhost"};
    uint16_t gameServerPort{25565};
    
    // Chat Server
    std::string chatServerHost{"localhost"};
    uint16_t chatServerPort{25566};
    
    // Quest Server
    std::string questServerHost{"localhost"};
    uint16_t questServerPort{25567};
    
    // Load from file
    bool loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Could not open config file: " << filename << std::endl;
            std::cerr << "Using default configuration (all servers on localhost)" << std::endl;
            return false;
        }
        
        std::string line;
        while (std::getline(file, line)) {
            // Skip comments and empty lines
            if (line.empty() || line[0] == '#') {
                continue;
            }
            
            // Parse key=value
            size_t equalPos = line.find('=');
            if (equalPos == std::string::npos) {
                continue;
            }
            
            std::string key = line.substr(0, equalPos);
            std::string value = line.substr(equalPos + 1);
            
            // Trim whitespace
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);
            
            // Set configuration values
            if (key == "loginServerHost") loginServerHost = value;
            else if (key == "loginServerPort") loginServerPort = static_cast<uint16_t>(std::stoi(value));
            else if (key == "characterServerHost") characterServerHost = value;
            else if (key == "characterServerPort") characterServerPort = static_cast<uint16_t>(std::stoi(value));
            else if (key == "gameServerHost") gameServerHost = value;
            else if (key == "gameServerPort") gameServerPort = static_cast<uint16_t>(std::stoi(value));
            else if (key == "chatServerHost") chatServerHost = value;
            else if (key == "chatServerPort") chatServerPort = static_cast<uint16_t>(std::stoi(value));
            else if (key == "questServerHost") questServerHost = value;
            else if (key == "questServerPort") questServerPort = static_cast<uint16_t>(std::stoi(value));
        }
        
        return true;
    }
    
    // Save to file
    bool saveToFile(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Could not write config file: " << filename << std::endl;
            return false;
        }
        
        file << "# CloneMine Server Configuration\n";
        file << "# Servers can be distributed across different hosts\n\n";
        
        file << "# Login Server\n";
        file << "loginServerHost=" << loginServerHost << "\n";
        file << "loginServerPort=" << loginServerPort << "\n\n";
        
        file << "# Character Server\n";
        file << "characterServerHost=" << characterServerHost << "\n";
        file << "characterServerPort=" << characterServerPort << "\n\n";
        
        file << "# Game Server\n";
        file << "gameServerHost=" << gameServerHost << "\n";
        file << "gameServerPort=" << gameServerPort << "\n\n";
        
        file << "# Chat Server\n";
        file << "chatServerHost=" << chatServerHost << "\n";
        file << "chatServerPort=" << chatServerPort << "\n\n";
        
        file << "# Quest Server\n";
        file << "questServerHost=" << questServerHost << "\n";
        file << "questServerPort=" << questServerPort << "\n";
        
        return true;
    }
    
    void printConfig() const {
        std::cout << "\n=== Server Configuration ===\n";
        std::cout << "Login Server:     " << loginServerHost << ":" << loginServerPort << "\n";
        std::cout << "Character Server: " << characterServerHost << ":" << characterServerPort << "\n";
        std::cout << "Game Server:      " << gameServerHost << ":" << gameServerPort << "\n";
        std::cout << "Chat Server:      " << chatServerHost << ":" << chatServerPort << "\n";
        std::cout << "Quest Server:     " << questServerHost << ":" << questServerPort << "\n";
        std::cout << "===========================\n\n";
    }
};

} // namespace config
} // namespace clonemine
