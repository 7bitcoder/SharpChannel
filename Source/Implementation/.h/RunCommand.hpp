#pragma once
#include "IRunnable.hpp"
#include "Settings.hpp"
#include "Control.hpp"
#include "ReadOnlyChannel.hpp"
#include "GetObject.hpp"

namespace cm {

    class RunCommand final: public ReadOnlyChannel {
        public:
            static std::unique_ptr<RunCommand> getObject(const RunCommandSettings& settings, IChannelEventLoop* eventLoop);
            RunCommand(const RunCommandSettings& settings): _settings(std::move(settings)) {}
            virtual ~RunCommand() {}

            void run() override;
            void finish() override;
        private:
            RunCommandSettings _settings;
    };
}