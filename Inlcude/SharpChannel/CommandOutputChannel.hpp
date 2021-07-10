#pragma once
#include "Settings.hpp"
#include "IRunnable.hpp"
#include "Channels/ReadOnlyChannel.hpp"

namespace cm
{
    class CommandOutputChannel final : public ReadOnlyChannel, public IRunnable
    {
    public:
        using Ptr = std::shared_ptr<CommandOutputChannel>;

        static Ptr create(const RunCommandSettings &settings, IChannelEventLoop *eventLoop = nullptr);
        CommandOutputChannel(const RunCommandSettings &settings) : _settings(std::move(settings)) {}
        ~CommandOutputChannel() {}

        void run() final;
        void finish() final;

    private:
        RunCommandSettings _settings;
    };
}