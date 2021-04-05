#pragma once
#include <mutex>
#include "IProcessComunicator.hpp"
#include "Settings.hpp"
#include "Control.hpp"
#include "Subject.hpp"

namespace pm {
    class SocketServer final: public pm::Subject {
        public:
            SocketServer(const SocketServerSettings& settings) { _settings = settings; init(); }
            ~SocketServer();
            void init();
            void run() override;
            bool sendImpl(const std::string& msg) override;
        private:
            std::mutex guard;
            SocketServerSettings _settings;
    };
}