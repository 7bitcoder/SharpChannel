#pragma once
#include "Settings.hpp"
#include "IRunnable.hpp"
#include "Channels/IReadOnlyChannel.hpp"

namespace cm
{
    struct ICommandChannel : public virtual IReadOnlyChannel, public IRunnable
    {
        using Ptr = std::shared_ptr<ICommandChannel>;
    };
}