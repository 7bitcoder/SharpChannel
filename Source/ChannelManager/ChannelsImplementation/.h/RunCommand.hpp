#pragma once
#include "IRunnable.hpp"
#include "Settings.hpp"
#include "Control.hpp"
#include "ReadOnlyChannel.hpp"

namespace cm {
    class RunCommand final: public ReadOnlyChannel {
        public:
            RunCommand(const RunCommandSettings& settings): _settings(std::move(settings)) {}
            ~RunCommand() {}

            void run() override;
            void finish() override;
        private:
            RunCommandSettings _settings;
    };
}