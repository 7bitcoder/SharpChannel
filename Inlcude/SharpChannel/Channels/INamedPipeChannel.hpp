#pragma once
#include "Settings.hpp"
#include "IRunnable.hpp"
#include "Channels/IReadOnlyChannel.hpp"
#include "Channels/IWriteOnlyChannel.hpp"

namespace cm
{
    struct INamedPipeChannel : public virtual IReadOnlyChannel, public virtual IWriteOnlyChannel, public IRunnable
    {
        using Ptr = std::shared_ptr<INamedPipeChannel>;
    };
}