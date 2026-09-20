// Auction Server - Separate server for handling auction house functionality
// Port 25569

#include <iostream>
#include <memory>
#include <csignal>
#include <atomic>
#include <thread>
#include <chrono>
#include "trading/AuctionHouse.h"
#include "config/ServerConfig.h"

// Global pointers/variables so the signal handler can perform shutdown actions directly
static AuctionHouse* g_auctionHouse = nullptr;
static int g_secondsRunning = 0;
static std::atomic<bool> g_running{true};

void signalHandler(int signal) {
    if (signal == SIGINT || signal == SIGTERM) {
        std::cout << "\n[AuctionServer] Shutdown signal received (" << signal << ")" << std::endl;
        std::cout << "[AuctionServer] Shutting down..." << std::endl;
        
        if (g_auctionHouse) {
            // Save auction state immediately on signal
            g_auctionHouse->saveToFile("game_data/auctions/active_auctions.json");
            
            std::cout << "[AuctionServer] Final stats:" << std::endl;
            std::cout << "  Active auctions: " << g_auctionHouse->getActiveAuctionCount() << std::endl;
            std::cout << "  Total processed: " << g_auctionHouse->getTotalAuctionsProcessed() << std::endl;
            std::cout << "  Uptime: " << (g_secondsRunning / 60) << " minutes" << std::endl;
        }
        
        std::cout << "[AuctionServer] Shutdown complete" << std::endl;
        
        // Force immediate exit so it doesn't wait on the loop or boolean flags
        std::exit(0);
    }
}

int main(int argc, char* argv[]) {
    std::cout << "==================================" << std::endl;
    std::cout << "  CloneMine Auction Server" << std::endl;
    std::cout << "==================================" << std::endl;
    
    // Parse command line arguments
    int port = 25569; // Default auction server port
    std::string configFile = "server_config.txt";
    
    if (argc > 1) {
        port = std::atoi(argv[1]);
    }
    if (argc > 2) {
        configFile = argv[2];
    }
    
    // Load server configuration
    std::cout << "[AuctionServer] Loading configuration from: " << configFile << std::endl;
    clonemine::config::ServerConfig config;
    config.loadFromFile(configFile);
    
    // Display configuration
    std::cout << "\n[Configuration]" << std::endl;
    std::cout << "Auction Server Port: " << port << std::endl;
    std::cout << "Login Server: " << config.loginServerHost << ":" << config.loginServerPort << std::endl;
    std::cout << "Character Server: " << config.characterServerHost << ":" << config.characterServerPort << std::endl;
    std::cout << "Game Server: " << config.gameServerHost << ":" << config.gameServerPort << std::endl;
    std::cout << "Chat Server: " << config.chatServerHost << ":" << config.chatServerPort << std::endl;
    std::cout << "Quest Server: " << config.questServerHost << ":" << config.questServerPort << std::endl;
    std::cout << std::endl;
    
    // Create auction house and assign global pointer for signal handling
    auto auctionHouse = std::make_unique<AuctionHouse>();
    g_auctionHouse = auctionHouse.get();
    
    // Setup signal handlers after pointers are ready
    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);
    
    // Load existing auctions from disk
    auctionHouse->loadFromFile("game_data/auctions/active_auctions.json");
    
    std::cout << "[AuctionServer] Starting on port " << port << std::endl;
    std::cout << "[AuctionServer] Active auctions: " << auctionHouse->getActiveAuctionCount() << std::endl;
    std::cout << "[AuctionServer] Total processed: " << auctionHouse->getTotalAuctionsProcessed() << std::endl;
    std::cout << "[AuctionServer] Press Ctrl+C to stop" << std::endl;
    std::cout << std::endl;
    
    // Main server loop
    const float updateInterval = 1.0f; // Update every second
    auto lastUpdate = std::chrono::steady_clock::now();
    
    while (g_running) {
        auto now = std::chrono::steady_clock::now();
        float deltaTime = std::chrono::duration<float>(now - lastUpdate).count();
        
        if (deltaTime >= updateInterval) {
            lastUpdate = now;
            g_secondsRunning++;
            
            // Update auction expirations
            auctionHouse->update();
            
            // Periodic status report (every 60 seconds)
            if (g_secondsRunning % 60 == 0) {
                std::cout << "[AuctionServer] Status - Active: " 
                          << auctionHouse->getActiveAuctionCount()
                          << ", Processed: " << auctionHouse->getTotalAuctionsProcessed()
                          << ", Uptime: " << (g_secondsRunning / 60) << " minutes" << std::endl;
            }
            
            // Save auctions periodically (every 5 minutes)
            if (g_secondsRunning % 300 == 0) {
                auctionHouse->saveToFile("game_data/auctions/active_auctions.json");
            }
        }
        
        // Sleep briefly to remain responsive
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    return 0;
}