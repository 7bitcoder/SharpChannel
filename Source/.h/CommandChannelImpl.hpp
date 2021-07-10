#pragma once
#include "Settings.hpp"
#include "IRunnable.hpp"
#include "CommandChannel.hpp"

namespace cm
{
    class CommandChannelImpl final : public CommandChannel
    {
    public:
        using Ptr = std::shared_ptr<CommandChannel>;

        CommandChannelImpl(const RunCommandSettings &settings) : _settings(std::move(settings)) {}

        void run() final;
        void finish() final;

    private:
        RunCommandSettings _settings;
    };
}