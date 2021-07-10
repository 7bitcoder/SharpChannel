#pragma once
#include <iostream>
#include <memory>
#include "CommandChannel.hpp"

namespace cm
{
    class SharpChannel
    {
    public:
        //static std::unique_ptr<IChannelDataObserver> makeStdIO(const StdComunicatorSettings &settings, IChannelEventLoop *eventLoop = nullptr);

        // static CommandChannel::Ptr makeSystemCommand(const RunCommandSettings &settings, IChannelEventLoop *eventLoop = nullptr);

        //static std::unique_ptr<IChannelDataObserver> makeSocketServer(const SocketServerSettings &settings, IChannelEventLoop *eventLoop = nullptr);

        //static std::unique_ptr<IChannelDataObserver> makeSocketClient(const SocketClientSettings &settings, IChannelEventLoop *eventLoop = nullptr);
    };
}