#pragma once
#include "Settings.hpp"
#include "IRunnable.hpp"
#include "CommandOutputChannel.hpp"

namespace cm
{
    class CommandOutputChannelImpl final : public CommandOutputChannel
    {
    public:
        using Ptr = std::shared_ptr<CommandOutputChannelImpl>;

        CommandOutputChannelImpl(const RunCommandSettings &settings) : _settings(std::move(settings)) {}

        void run() final;
        void finish() final;

    private:
        RunCommandSettings _settings;
    };
}