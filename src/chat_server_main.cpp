#include "server/ChatServer.h"
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
    uint16_t port = 25566; // Default chat port (game server + 1)
    
    if (argc > 1) {
        try {
            port = static_cast<uint16_t>(std::stoi(argv[1]));
        } catch (...) {
            std::cerr << "Invalid port number. Using default: " << port << std::endl;
        }
    }
    
    std::cout << "CloneMine Chat Server\n";
    std::cout << "=====================\n";
    
    // Register signal handlers
    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);
    
    try {
        clonemine::server::ChatServer server(port);
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
