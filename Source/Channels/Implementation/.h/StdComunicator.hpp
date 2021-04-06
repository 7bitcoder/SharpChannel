#pragma once
#include "IRunnable.hpp"
#include "Settings.hpp"
#include "Control.hpp"
#include "ReadOnlyChannel.hpp"

namespace cm {
    class StdComunicator final: public ReadOnlyChannel {
        public:
            StdComunicator(const StdChannelSettings& settings) { _settings = settings; }
            ~StdComunicator() {}

            void run() override;
        private:
            StdChannelSettings _settings;
    };
}