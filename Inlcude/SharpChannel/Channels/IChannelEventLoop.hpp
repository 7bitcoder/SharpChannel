#pragma once
#include <functional>

namespace cm
{
    struct IChannelEventLoop
    {
        using Ptr = std::shared_ptr<IChannelEventLoop>;
     
        virtual void postChannelEvent(const std::function<void(void)> event) = 0;

        virtual ~IChannelEventLoop() {}
    };

}