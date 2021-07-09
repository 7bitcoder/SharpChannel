#pragma once
#include <mutex>
#include "IRunnable.hpp"
#include "Settings.hpp"

#include "GetObject.hpp"

namespace cm
{
    class SocketClient 
    {
    public:
        static std::unique_ptr<SocketClient> getObject(const SocketClientSettings &settings, IChannelEventLoop *eventLoop);

        ~SocketClient() {}
    };
}