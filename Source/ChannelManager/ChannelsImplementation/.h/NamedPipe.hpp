#pragma once
#include "IRunnable.hpp"
#include "Settings.hpp"
#include "Control.hpp"
#include "Channel.hpp"

namespace cm {
    class NamedPipe final: public Channel {
        public:
            NamedPipe(const NamedPipeSettings& settings) { _settings = settings; }
            ~NamedPipe() {}

            void run() override;
            bool sendImpl(const std::string& msg) override;
            bool sendImpl(const char* data, size_t lenght) override;
        private:
            NamedPipeSettings _settings;
    };
}