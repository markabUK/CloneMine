#include "server/QuestServer.h"
#include "config/ServerConfig.h"
#include <iostream>
#include <csignal>
#include <atomic>
#include <thread>
#include <chrono>

// Global server pointer for safe signal handling
clonemine::server::QuestServer* g_server = nullptr;

void signalHandler(int signal) {
    std::cout << "\nReceived signal " << signal << ", shutting down..." << std::endl;
    if (g_server) {
        g_server->stop(); // Unblocks server.run() and stops the network event loop
    }
}

int main(int argc, char* argv[]) {
    uint16_t port = 25567; // Default quest port
    std::string configFile = "server_config.txt";
    
    if (argc > 1) {
        try {
            port = static_cast<uint16_t>(std::stoi(argv[1]));
        } catch (...) {
            std::cerr << "Invalid port number. Using default: " << port << std::endl;
        }
    }
    
    if (argc > 2) {
        configFile = argv[2];
    }
    
    // Load configuration
    clonemine::config::ServerConfig config;
    config.loadFromFile(configFile);
    
    std::cout << "CloneMine Quest Server\n";
    std::cout << "======================\n";
    config.printConfig();
    std::cout << "Quest server can be queried by Game Server and clients\n";
    std::cout << std::endl;
    
    try {
        clonemine::server::QuestServer server(port);
        g_server = &server; // Assign global pointer
        
        // Register signal handlers after server is initialized
        std::signal(SIGINT, signalHandler);
        std::signal(SIGTERM, signalHandler);
        
        server.start();
        server.run(); // Blocks until server.stop() is called by the signal handler
        
        g_server = nullptr; // Cleanup
        
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}