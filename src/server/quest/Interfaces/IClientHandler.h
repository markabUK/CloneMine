#ifndef QUEST_ICLIENT_HANDLER_H
#define QUEST_ICLIENT_HANDLER_H

namespace CloneMine {
namespace Quest {

class IClientHandler {
public:
    virtual ~IClientHandler() = default;
    
    virtual void HandleClient() = 0;
    virtual void Stop() = 0;
};

} // namespace Quest
} // namespace CloneMine

#endif
