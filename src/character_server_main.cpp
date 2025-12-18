#include "server/CharacterServer.h"
#include "config/ServerConfig.h"
#include <iostream>
#include <csignal>
#include <atomic>

std::atomic<bool> g_running{true};

void signalHandler(int signal) {
    (void)signal;
    std::cout << "\nReceived signal " << signal << ", shutting down..." << std::endl;
    g_running = false;
}

int main(int argc, char* argv[]) {
    uint16_t port = 25568; // Default character port
    uint32_t maxCharacters = 5; // Default max characters per account
    std::string configFile = "server_config.txt";
    
    if (argc > 1) {
        try {
            port = static_cast<uint16_t>(std::stoi(argv[1]));
        } catch (...) {
            std::cerr << "Invalid port number. Using default: " << port << std::endl;
        }
    }
    
    if (argc > 2) {
        try {
            maxCharacters = static_cast<uint32_t>(std::stoi(argv[2]));
        } catch (...) {
            std::cerr << "Invalid max characters. Using default: " << maxCharacters << std::endl;
        }
    }
    
    if (argc > 3) {
        configFile = argv[3];
    }
    
    // Load configuration (for inter-server communication)
    clonemine::config::ServerConfig config;
    config.loadFromFile(configFile);
    
    std::cout << "CloneMine Character Server\n";
    std::cout << "==========================\n";
    config.printConfig();
    std::cout << "Port: " << port << "\n";
    std::cout << "Max characters per account: " << maxCharacters << "\n";
    std::cout << "\nNOTE: Character server provides state to Game Server at "
              << config.gameServerHost << ":" << config.gameServerPort << "\n";
    std::cout << std::endl;
    
    // Register signal handlers
    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);
    
    try {
        clonemine::server::CharacterServer server(port, maxCharacters);
        server.start();
        server.run();
        
        // Wait for shutdown signal
        while (g_running && server.isRunning()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        
        server.stop();
        
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
