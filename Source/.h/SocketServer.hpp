#pragma once
#include <mutex>
#include "IRunnable.hpp"
#include "Settings.hpp"

#include "GetObject.hpp"

namespace cm
{
    class SocketServer 
    {
    public:
        static std::unique_ptr<SocketServer> getObject(const SocketServerSettings &settings, IChannelEventLoop *eventLoop);

        ~SocketServer() {}
    };
}