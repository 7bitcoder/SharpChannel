#pragma once
#include "Settings.hpp"
#include "Channels/ICommandChannel.hpp"
#include "ReadOnlyChannel.hpp"

namespace cm
{
    class CommandChannelMulti final : public ICommandChannel, public ReadOnlyChannel
    {
    public:
        using Ptr = std::shared_ptr<CommandChannelMulti>;

        CommandChannelMulti(const RunCommandSettings &settings) : _settings(std::move(settings)) {}

        void run() final;
        void finish() final;

    private:
        RunCommandSettings _settings;
    };
}