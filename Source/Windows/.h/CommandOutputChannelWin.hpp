#pragma once
#include "Settings.hpp"
#include "ReadOnlyChannel.hpp"
#include "Channels/ICommandOutputChannel.hpp"

namespace cm
{
    class CommandOutputChannelWin final : public ICommandOutputChannel, public ReadOnlyChannel
    {
    public:
        using Ptr = std::shared_ptr<CommandOutputChannelWin>;

        CommandOutputChannelWin(const RunCommandSettings &settings) : _settings(std::move(settings)) {}

        void run() final;
        void finish() final;

    private:
        RunCommandSettings _settings;
    };
}