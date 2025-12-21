#pragma once

#include <string>
#include <utility>
#include <memory>

namespace CloneMine {
namespace Chat {

/**
 * @brief Interface for input validation operations
 * 
 * Follows Interface Segregation Principle - focused contract for validation
 */
class IInputValidator {
public:
    virtual ~IInputValidator() = default;

    virtual std::pair<bool, std::string> ValidateSender(const std::string& sender) = 0;
    virtual std::pair<bool, std::string> ValidateMessage(const std::string& message) = 0;
    virtual std::pair<bool, std::string> ValidateCommand(const std::string& command) = 0;
};

} // namespace Chat
} // namespace CloneMine
