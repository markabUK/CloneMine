#pragma once

#include "../Interfaces/IAccountRepository.h"
#include "../Models/Account.h"
#include <unordered_map>
#include <memory>
#include <mutex>
#include <string>

namespace CloneMine {
namespace Login {
namespace Repositories {

/**
 * @brief In-memory account repository implementation
 * Thread-safe storage for user accounts.
 */
class InMemoryAccountRepository : public IAccountRepository {
private:
    std::unordered_map<std::string, std::shared_ptr<Account>> accounts_;
    mutable std::mutex mutex_;

public:
    InMemoryAccountRepository() = default;

    std::shared_ptr<Account> GetByUsername(const std::string& username) override {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = accounts_.find(username);
        if (it != accounts_.end()) {
            return it->second;
        }
        return nullptr;
    }

    std::shared_ptr<Account> GetById(int id) override {
        std::lock_guard<std::mutex> lock(mutex_);
        for (const auto& pair : accounts_) {
            if (pair.second->Id == id) {
                return pair.second;
            }
        }
        return nullptr;
    }

    bool Create(std::shared_ptr<Account> account) override {
        if (!account || account->Username.empty()) {
            return false;
        }

        std::lock_guard<std::mutex> lock(mutex_);
        
        // Check if username already exists
        if (accounts_.find(account->Username) != accounts_.end()) {
            return false;
        }

        // Auto-increment ID
        int maxId = 0;
        for (const auto& pair : accounts_) {
            if (pair.second->Id > maxId) {
                maxId = pair.second->Id;
            }
        }
        account->Id = maxId + 1;

        accounts_[account->Username] = account;
        return true;
    }

    bool Update(std::shared_ptr<Account> account) override {
        if (!account || account->Username.empty()) {
            return false;
        }

        std::lock_guard<std::mutex> lock(mutex_);
        
        auto it = accounts_.find(account->Username);
        if (it == accounts_.end()) {
            return false;
        }

        it->second = account;
        return true;
    }

    bool Delete(const std::string& username) override {
        std::lock_guard<std::mutex> lock(mutex_);
        return accounts_.erase(username) > 0;
    }

    bool Exists(const std::string& username) override {
        std::lock_guard<std::mutex> lock(mutex_);
        return accounts_.find(username) != accounts_.end();
    }
};

} // namespace Repositories
} // namespace Login
} // namespace CloneMine
