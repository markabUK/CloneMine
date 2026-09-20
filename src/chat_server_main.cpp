#include "server/ChatServer.h"
#include "config/ServerConfig.h"
#include <iostream>
#include <csignal>
#include <atomic>
#include <thread>
#include <chrono>

// Global server pointer so the signal handler can safely stop it
clonemine::server::ChatServer* g_server = nullptr;

void signalHandler(int signal) {
    std::cout << "\nReceived signal " << signal << ", shutting down..." << std::endl;
    if (g_server) {
        g_server->stop(); // This unblocks server.run() and shuts down the network loop
    }
}

int main(int argc, char* argv[]) {
    uint16_t port = 25566; // Default chat port
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
    
    std::cout << "CloneMine Chat Server\n";
    std::cout << "=====================\n";
    config.printConfig();
    std::cout << "Chat server can run on any host - clients connect directly\n";
    std::cout << "Character names provided by clients during connection\n";
    std::cout << std::endl;
    
    try {
        clonemine::server::ChatServer server(port);
        g_server = &server; // Assign global pointer
        
        // Register signal handlers after server is initialized
        std::signal(SIGINT, signalHandler);
        std::signal(SIGTERM, signalHandler);
        
        server.start();
        server.run(); // This will now properly block until server.stop() is called by the signal handler
        
        g_server = nullptr; // Cleanup
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}