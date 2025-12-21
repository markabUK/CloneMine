#pragma once

#include "../Interfaces/IGameService.h"
#include "../Interfaces/IPlayerRepository.h"
#include "../Interfaces/IInputValidator.h"
#include "../Models/Player.h"
#include "../Models/ServerConfiguration.h"
#include <memory>
#include <thread>
#include <atomic>
#include <chrono>
#include <cstdlib>
#include <sstream>
#include <iomanip>

namespace CloneMine {
namespace Game {
namespace Services {

class GameService : public Interfaces::IGameService {
private:
    std::shared_ptr<Interfaces::IPlayerRepository> playerRepository;
    std::shared_ptr<Interfaces::IInputValidator> validator;
    std::shared_ptr<Models::ServerConfiguration> config;
    std::thread gameLoopThread;
    std::atomic<bool> running;

    std::string GeneratePlayerId() {
        std::stringstream ss;
        ss << std::hex << std::setfill('0');
        for (int i = 0; i < 16; ++i) {
            ss << std::setw(2) << (rand() % 256);
        }
        return ss.str();
    }

    void RunGameLoop() {
        while (running) {
            auto start = std::chrono::steady_clock::now();
            
            UpdateGameState();
            
            // Maintain tick rate
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now() - start);
            auto sleepTime = std::chrono::milliseconds(config->GetTickIntervalMs()) - elapsed;
            
            if (sleepTime.count() > 0) {
                std::this_thread::sleep_for(sleepTime);
            }
        }
    }

public:
    GameService(std::shared_ptr<Interfaces::IPlayerRepository> repository,
                std::shared_ptr<Interfaces::IInputValidator> inputValidator,
                std::shared_ptr<Models::ServerConfiguration> configuration)
        : playerRepository(repository),
          validator(inputValidator),
          config(configuration),
          running(false) {
    }

    ~GameService() {
        if (running) {
            StopGameLoop();
        }
    }

    bool JoinGame(const std::string& playerId, const std::string& username) override {
        auto validation = validator->ValidateUsername(username);
        if (!validation.first) {
            return false;
        }

        if (playerRepository->GetPlayerCount() >= config->MaxPlayers) {
            return false;
        }

        auto player = std::make_shared<Models::Player>(playerId, username);
        return playerRepository->AddPlayer(player);
    }

    bool LeaveGame(const std::string& playerId) override {
        return playerRepository->RemovePlayer(playerId);
    }

    bool MovePlayer(const std::string& playerId, double x, double y, double z) override {
        auto validation = validator->ValidateCoordinates(x, y, z);
        if (!validation.first) {
            return false;
        }

        auto player = playerRepository->GetPlayer(playerId);
        if (!player) {
            return false;
        }

        player->UpdatePosition(x, y, z);
        player->State = 1;  // Moving
        return playerRepository->UpdatePlayer(player);
    }

    bool AttackPlayer(const std::string& attackerId, const std::string& targetId) override {
        auto attacker = playerRepository->GetPlayer(attackerId);
        auto target = playerRepository->GetPlayer(targetId);

        if (!attacker || !target) {
            return false;
        }

        if (!attacker->IsAlive() || !target->IsAlive()) {
            return false;
        }

        // Simple attack logic: deal 10 damage
        target->TakeDamage(10);
        attacker->State = 2;  // Attacking

        playerRepository->UpdatePlayer(attacker);
        playerRepository->UpdatePlayer(target);
        return true;
    }

    std::shared_ptr<Models::Player> GetPlayerState(const std::string& playerId) override {
        return playerRepository->GetPlayer(playerId);
    }

    void UpdateGameState() override {
        // Game loop logic: update all players
        auto players = playerRepository->GetAllPlayers();
        for (auto& player : players) {
            // Reset state to idle if not dead
            if (player->State != 3 && player->State != 0) {
                player->State = 0;
                playerRepository->UpdatePlayer(player);
            }

            // Auto-heal slowly (1 HP per tick if alive)
            if (player->IsAlive() && player->Health < 100) {
                player->Heal(1);
                playerRepository->UpdatePlayer(player);
            }
        }
    }

    void StartGameLoop() override {
        if (!running) {
            running = true;
            gameLoopThread = std::thread(&GameService::RunGameLoop, this);
        }
    }

    void StopGameLoop() override {
        if (running) {
            running = false;
            if (gameLoopThread.joinable()) {
                gameLoopThread.join();
            }
        }
    }
};

} // namespace Services
} // namespace Game
} // namespace CloneMine
