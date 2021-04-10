#pragma once
#include <mutex>
#include "IRunnable.hpp"
#include "Settings.hpp"
#include "Control.hpp"
#include "GetObject.hpp"
#include "Channel.hpp"

namespace cm
{
    class SocketServer : public Channel
    {
    public:
        static std::unique_ptr<SocketServer> getObject(const SocketServerSettings &settings, IChannelEventLoop *eventLoop);

        virtual ~SocketServer() {}
    };
}