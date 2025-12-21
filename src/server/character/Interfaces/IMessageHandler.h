#pragma once
#include <string>

class IMessageHandler {
public:
    virtual ~IMessageHandler() = default;
    
    virtual std::string HandleMessage(const std::string& message) = 0;
};
