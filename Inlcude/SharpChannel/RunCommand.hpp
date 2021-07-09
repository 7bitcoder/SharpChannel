#pragma once
#include "Settings.hpp"
#include "IRunnable.hpp"
#include "Channels/ReadOnlyChannel.hpp"

namespace cm
{
    class RunCommand final : public ReadOnlyChannel, public IRunnable
    {
    public:
        using Ptr = std::unique_ptr<RunCommand>;

        static Ptr create(const RunCommandSettings &settings, IChannelEventLoop *eventLoop);
        RunCommand(const RunCommandSettings &settings) : _settings(std::move(settings)) {}
        ~RunCommand() {}

        void run() final;
        void finish() final;

    private:
        RunCommandSettings _settings;
    };
}