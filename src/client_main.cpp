#include "client/ClientApplication.h"
#include "config/ServerConfig.h"
#include <iostream>
#include <stdexcept>

int main(int argc, char* argv[]) {
    std::string characterName = "Player";
    std::string configFile = "server_config.txt";
    
    // Parse command line arguments
    if (argc > 1) {
        characterName = argv[1];
    }
    if (argc > 2) {
        configFile = argv[2];
    }
    
    // Load server configuration (supports distributed servers)
    clonemine::config::ServerConfig config;
    config.loadFromFile(configFile);
    config.printConfig();
    
    try {
        std::cout << "CloneMine Client" << std::endl;
        std::cout << "================" << std::endl;
        std::cout << "Character: " << characterName << "\n\n";
        
        clonemine::client::ClientApplication app("CloneMine - Client", 1280, 720);
        
        // Set character name (will be used for chat)
        app.setCharacterName(characterName);
        
        // In a full implementation, would:
        // 1. Connect to Login Server (config.loginServerHost:loginServerPort)
        // 2. Authenticate and get session token
        // 3. Connect to Character Server (config.characterServerHost:characterServerPort)
        // 4. Select/create character and get character data
        // 5. Connect to Game Server with character data
        // 6. Connect to Chat Server with character name
        // 7. Connect to Quest Server for quest data
        
        // For now, connect to game server directly
        if (!app.connectToServer(config.gameServerHost, config.gameServerPort, characterName)) {
            std::cerr << "Failed to connect to game server at " 
                      << config.gameServerHost << ":" << config.gameServerPort << std::endl;
            return EXIT_FAILURE;
        }
        
        std::cout << "Connected to game server! Starting game..." << std::endl;
        app.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
