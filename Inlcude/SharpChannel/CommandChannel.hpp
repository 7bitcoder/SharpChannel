#pragma once
#include "Settings.hpp"
#include "IRunnable.hpp"
#include "Channels/ReadOnlyChannel.hpp"

namespace cm
{
    class CommandChannel : public ReadOnlyChannel, public IRunnable
    {
    public:
        using Ptr = std::shared_ptr<CommandChannel>;

        static Ptr create(const RunCommandSettings &settings, IChannelEventLoop *eventLoop = nullptr);
    };
}