#pragma once

#include <string>
#include <utility>

namespace CloneMine {
namespace Game {
namespace Interfaces {

class IInputValidator {
public:
    virtual ~IInputValidator() = default;
    
    virtual std::pair<bool, std::string> ValidatePlayerId(const std::string& playerId) = 0;
    virtual std::pair<bool, std::string> ValidateUsername(const std::string& username) = 0;
    virtual std::pair<bool, std::string> ValidateCoordinate(double coordinate) = 0;
    virtual std::pair<bool, std::string> ValidateCoordinates(double x, double y, double z) = 0;
};

} // namespace Interfaces
} // namespace Game
} // namespace CloneMine
