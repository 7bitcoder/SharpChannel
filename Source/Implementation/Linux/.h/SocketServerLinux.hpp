#pragma once
#include <mutex>
#include "IRunnable.hpp"
#include "Settings.hpp"

#include "Channel.hpp"
#include "SocketServer.hpp"

namespace cm {
    /*class SocketServerLinux final: public SocketServer {
        public:
            SocketServerLinux(const SocketServerSettings& settings) { _settings = settings; init(); }
            virtual ~SocketServerLinux();
            void init();
            void run() override;

            bool sendMessageImpl(const std::string& msg) override;
            bool sendDataImpl(const std::vector<char>& data) override;
        private:
            std::mutex guard;
            SocketServerSettings _settings;
    };*/
}