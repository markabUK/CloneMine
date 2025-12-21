#pragma once

#include "../Interfaces/IAuthenticationService.h"
#include "../Interfaces/IAccountRepository.h"
#include "../Interfaces/IPasswordHasher.h"
#include "../Interfaces/IInputValidator.h"
#include "../Models/Account.h"
#include "../Models/LoginResult.h"
#include <memory>
#include <string>
#include <random>
#include <sstream>
#include <iomanip>

namespace CloneMine {
namespace Login {
namespace Services {

/**
 * @brief Authentication service implementation
 * Handles user registration, login, and session management.
 */
class AuthenticationService : public IAuthenticationService {
private:
    std::shared_ptr<IAccountRepository> accountRepository_;
    std::shared_ptr<IPasswordHasher> passwordHasher_;
    std::shared_ptr<IInputValidator> inputValidator_;

    /**
     * @brief Generate random session token
     * @return 128-bit hex string
     */
    std::string GenerateSessionToken() {
        std::random_device rd;
        std::mt19937_64 gen(rd());
        std::uniform_int_distribution<uint64_t> dis;

        uint64_t part1 = dis(gen);
        uint64_t part2 = dis(gen);

        std::ostringstream oss;
        oss << std::hex << std::setfill('0') 
            << std::setw(16) << part1 
            << std::setw(16) << part2;

        return oss.str();
    }

public:
    AuthenticationService(std::shared_ptr<IAccountRepository> accountRepository,
                         std::shared_ptr<IPasswordHasher> passwordHasher,
                         std::shared_ptr<IInputValidator> inputValidator)
        : accountRepository_(accountRepository),
          passwordHasher_(passwordHasher),
          inputValidator_(inputValidator) {
    }

    LoginResult Login(const std::string& username, const std::string& password) override {
        // Validate username
        auto usernameValidation = inputValidator_->ValidateUsername(username);
        if (!usernameValidation.first) {
            return LoginResult{false, usernameValidation.second, "", nullptr};
        }

        // Validate password
        auto passwordValidation = inputValidator_->ValidatePassword(password);
        if (!passwordValidation.first) {
            return LoginResult{false, passwordValidation.second, "", nullptr};
        }

        // Get account
        auto account = accountRepository_->GetByUsername(username);
        if (!account) {
            return LoginResult{false, "Invalid username or password", "", nullptr};
        }

        // Verify password
        if (!passwordHasher_->VerifyPassword(password, account->PasswordHash)) {
            return LoginResult{false, "Invalid username or password", "", nullptr};
        }

        // Generate session token
        std::string sessionToken = GenerateSessionToken();

        return LoginResult{true, "Login successful", sessionToken, account};
    }

    LoginResult Register(const std::string& username, const std::string& password) override {
        // Validate username
        auto usernameValidation = inputValidator_->ValidateUsername(username);
        if (!usernameValidation.first) {
            return LoginResult{false, usernameValidation.second, "", nullptr};
        }

        // Validate password
        auto passwordValidation = inputValidator_->ValidatePassword(password);
        if (!passwordValidation.first) {
            return LoginResult{false, passwordValidation.second, "", nullptr};
        }

        // Check if username already exists
        if (accountRepository_->Exists(username)) {
            return LoginResult{false, "Username already exists", "", nullptr};
        }

        // Hash password
        std::string passwordHash;
        try {
            passwordHash = passwordHasher_->HashPassword(password);
        }
        catch (const std::exception& e) {
            return LoginResult{false, "Failed to hash password: " + std::string(e.what()), "", nullptr};
        }

        // Create account
        auto account = std::make_shared<Account>();
        account->Username = username;
        account->PasswordHash = passwordHash;
        account->CreatedAt = std::time(nullptr);
        account->LastLoginAt = std::time(nullptr);

        if (!accountRepository_->Create(account)) {
            return LoginResult{false, "Failed to create account", "", nullptr};
        }

        // Generate session token
        std::string sessionToken = GenerateSessionToken();

        return LoginResult{true, "Registration successful", sessionToken, account};
    }

    bool ValidateSession(const std::string& sessionToken) override {
        auto validation = inputValidator_->ValidateSessionToken(sessionToken);
        return validation.first;
    }

    bool Logout(const std::string& sessionToken) override {
        // In a real implementation, this would remove the session from a session store
        // For now, just validate the token format
        return ValidateSession(sessionToken);
    }
};

} // namespace Services
} // namespace Login
} // namespace CloneMine
