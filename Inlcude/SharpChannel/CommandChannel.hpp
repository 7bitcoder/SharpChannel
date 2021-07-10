#pragma once
#include "Settings.hpp"
#include "IRunnable.hpp"
#include "Channels/ReadOnlyChannel.hpp"

namespace cm
{
    class CommandChannel final : public ReadOnlyChannel, public IRunnable
    {
    public:
        using Ptr = std::unique_ptr<CommandChannel>;

        static Ptr create(const RunCommandSettings &settings, IChannelEventLoop *eventLoop = nullptr);
        CommandChannel(const RunCommandSettings &settings) : _settings(std::move(settings)) {}
        ~CommandChannel() {}

        void run() final;
        void finish() final;

    private:
        RunCommandSettings _settings;
    };
}