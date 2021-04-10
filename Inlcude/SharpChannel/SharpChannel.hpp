#pragma once
#include <iostream>
#include <memory>
#include "Control.hpp"
#include "Settings.hpp"
#include "IRunnable.hpp"
#include "IObserver.hpp"
#include "IMessageObserver.hpp"
#include "IDataObserver.hpp"
#include "IChannel.hpp"
#include "IReadOnlyChannel.hpp"
#include "IWriteOnlyChannel.hpp"
#include "IUnsubscribable.hpp"
#include "IChannelEventLoop.hpp"

namespace cm {
    class ChannelException: public std::exception {
        public:
            explicit ChannelException(const char* message)
                : msg_(message) {}

            explicit ChannelException(const std::string& message)
                : msg_(message) {}

            virtual ~ChannelException() noexcept {}

            virtual const char* what() const noexcept { return msg_.c_str(); }

        protected:
            std::string msg_;
    };
    
    class SharpChannel {
        public:
            static std::unique_ptr<IReadOnlyChannel> makeStdIO(const StdComunicatorSettings& settings, IChannelEventLoop* eventLoop = nullptr);

            static std::unique_ptr<IReadOnlyChannel> makeSystemCommand(const RunCommandSettings& settings, IChannelEventLoop* eventLoop = nullptr);

            static std::unique_ptr<IChannel> makeSocketServer(const SocketServerSettings& settings, IChannelEventLoop* eventLoop = nullptr);
    };
}