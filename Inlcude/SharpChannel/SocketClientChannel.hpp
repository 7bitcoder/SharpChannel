#pragma once
#include <mutex>
#include "IRunnable.hpp"
#include "Settings.hpp"
#include "FullDuplexChannel.hpp"

namespace cm
{
    class SocketClientChannel : public FullDuplexChannel, public IRunnable
    {
    public:
        using Ptr = std::shared_ptr<SocketClientChannel>;

        static Ptr create(const SocketClientSettings &settings, IChannelEventLoop *eventLoop = nullptr);
    };
}