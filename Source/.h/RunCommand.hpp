#pragma once
#include "IRunCommand.hpp"
#include "Settings.hpp"
#include "ChannelObservable.hpp"
#include "GetObject.hpp"

namespace cm
{
    class RunCommand final : public ChannelObservable, public IRunCommand
    {
    public:
        static std::unique_ptr<RunCommand> getObject(const RunCommandSettings &settings, IChannelEventLoop *eventLoop);
        RunCommand(const RunCommandSettings &settings) : _settings(std::move(settings)) {}
        ~RunCommand() {}

        void run() final;
        void finish() final;

    private:
        RunCommandSettings _settings;
    };
}