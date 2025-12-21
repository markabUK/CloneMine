#ifndef QUEST_IMESSAGE_HANDLER_H
#define QUEST_IMESSAGE_HANDLER_H

#include <string>

namespace CloneMine {
namespace Quest {

class IMessageHandler {
public:
    virtual ~IMessageHandler() = default;
    
    virtual std::string HandleMessage(const std::string& message) = 0;
};

} // namespace Quest
} // namespace CloneMine

#endif
