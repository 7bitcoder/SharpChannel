#pragma once 
#include <functional>

namespace cm {
    struct IChannelEventLoop
    {
        virtual void postChannelEvent(const std::function<void(void)> event) = 0;

        virtual ~IChannelEventLoop() {}
    };
    
}