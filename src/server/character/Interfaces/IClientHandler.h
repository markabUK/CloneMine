#pragma once
#include <memory>

class IClientHandler {
public:
    virtual ~IClientHandler() = default;
    
    virtual void HandleClient(int clientSocket) = 0;
};
