#pragma once
#include "IChannelObservable.hpp"
#include "IRunnable.hpp"

namespace cm
{

    struct IRunCommand: public virtual IChannelObservable, public IRunnable
    {
        ~IRunCommand() {}
    };
}