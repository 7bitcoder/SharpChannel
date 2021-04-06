#pragma once
#include <iostream>
#include <functional>
#include <memory>
#include "IRunnable.hpp"
#include "StdComunicator.hpp"
#include "SocketServer.hpp"
#include "Settings.hpp"

#ifdef WIN
    
#endif

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
    
    class ChannelManager {
        public:
            static std::unique_ptr<IReadOnlyChannel> makeStdIO(const StdChannelSettings& settings);

            static std::unique_ptr<IReadOnlyChannel> makeSystemCommand(const RunCommandSettings& settings);

            static std::unique_ptr<IChannel> makeSocketServer(const SocketServerSettings& settings);
    };
}