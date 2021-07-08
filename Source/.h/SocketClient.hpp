#pragma once
#include <mutex>
#include "IRunnable.hpp"
#include "Settings.hpp"

#include "GetObject.hpp"
#include "ChannelObservable.hpp"

namespace cm
{
    class SocketClient : public ChannelObservable
    {
    public:
        static std::unique_ptr<SocketClient> getObject(const SocketClientSettings &settings, IChannelEventLoop *eventLoop);

        ~SocketClient() {}
    };
}