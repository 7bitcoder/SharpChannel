#pragma once
#include "Settings.hpp"
#include "IRunnable.hpp"
#include "Channels/IReadOnlyChannel.hpp"

namespace cm
{
    struct ICommandOutputChannel : public virtual IReadOnlyChannel, public IRunnable
    {
        using Ptr = std::shared_ptr<ICommandOutputChannel>;
    };
}