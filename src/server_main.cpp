#include "server/GameServer.h"
#include <iostream>
#include <csignal>
#include <memory>

std::unique_ptr<clonemine::server::GameServer> g_server;

void signalHandler(int signal) {
    if (g_server) {
        std::cout << "\nReceived signal " << signal << ", shutting down..." << std::endl;
        g_server->stop();
    }
}

int main(int argc, char* argv[]) {
    uint16_t port = 25565; // Default Minecraft port
    
    if (argc > 1) {
        try {
            port = static_cast<uint16_t>(std::stoi(argv[1]));
        } catch (...) {
            std::cerr << "Invalid port number. Using default: " << port << std::endl;
        }
    }
    
    try {
        // Register signal handlers
        std::signal(SIGINT, signalHandler);
        std::signal(SIGTERM, signalHandler);
        
        std::cout << "CloneMine Server" << std::endl;
        std::cout << "=================" << std::endl;
        
        g_server = std::make_unique<clonemine::server::GameServer>(port);
        g_server->start();
        g_server->run();
        
        return EXIT_SUCCESS;
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}
