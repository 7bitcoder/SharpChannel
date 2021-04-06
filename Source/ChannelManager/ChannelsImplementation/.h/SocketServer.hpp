#pragma once
#include <mutex>
#include "IRunnable.hpp"
#include "Settings.hpp"
#include "Control.hpp"
#include "Channel.hpp"

namespace cm {
    class SocketServer final: public Channel {
        public:
            SocketServer(const SocketServerSettings& settings) { _settings = settings; init(); }
            ~SocketServer();
            void init();
            void run() override;
            
            bool sendImpl(const char* data, size_t lenght) override;
            bool sendImpl(const std::string& msg) override;
        private:
            std::mutex guard;
            SocketServerSettings _settings;
    };
}