#pragma once
#include <iostream>
#include <memory>

#include "Settings.hpp"
#include "IMessageObserver.hpp"
#include "IChannelObservable.hpp"
#include "IUnsubscribable.hpp"
#include "IChannelEventLoop.hpp"
#include "IRunCommand.hpp"

namespace cm
{
    class ChannelException : public std::exception
    {
    public:
        explicit ChannelException(const char *message)
            : msg_(message) {}

        explicit ChannelException(const std::string &message)
            : msg_(message) {}

        virtual ~ChannelException() noexcept {}

        virtual const char *what() const noexcept { return msg_.c_str(); }

    protected:
        std::string msg_;
    };

    class SharpChannel
    {
    public:
        //static std::unique_ptr<IChannelDataObserver> makeStdIO(const StdComunicatorSettings &settings, IChannelEventLoop *eventLoop = nullptr);

        static std::unique_ptr<IRunCommand> makeSystemCommand(const RunCommandSettings &settings, IChannelEventLoop *eventLoop = nullptr);

        //static std::unique_ptr<IChannelDataObserver> makeSocketServer(const SocketServerSettings &settings, IChannelEventLoop *eventLoop = nullptr);

        //static std::unique_ptr<IChannelDataObserver> makeSocketClient(const SocketClientSettings &settings, IChannelEventLoop *eventLoop = nullptr);
    };
}