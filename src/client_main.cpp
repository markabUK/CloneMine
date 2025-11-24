#include "client/ClientApplication.h"
#include <iostream>
#include <stdexcept>

int main(int argc, char* argv[]) {
    std::string host = "localhost";
    uint16_t port = 25565;
    std::string playerName = "Player";
    
    // Parse command line arguments
    if (argc > 1) {
        host = argv[1];
    }
    if (argc > 2) {
        try {
            port = static_cast<uint16_t>(std::stoi(argv[2]));
        } catch (...) {
            std::cerr << "Invalid port number. Using default: " << port << std::endl;
        }
    }
    if (argc > 3) {
        playerName = argv[3];
    }
    
    try {
        std::cout << "CloneMine Client" << std::endl;
        std::cout << "================" << std::endl;
        
        clonemine::client::ClientApplication app("CloneMine - Client", 1280, 720);
        
        // Connect to server
        if (!app.connectToServer(host, port, playerName)) {
            std::cerr << "Failed to connect to server at " << host << ":" << port << std::endl;
            return EXIT_FAILURE;
        }
        
        std::cout << "Connected! Starting game..." << std::endl;
        app.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
