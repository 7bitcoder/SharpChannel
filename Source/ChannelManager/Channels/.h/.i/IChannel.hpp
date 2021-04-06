#pragma once
#include <iostream>
#include "IWriteOnlyChannel.hpp"
#include "IReadOnlyChannel.hpp"

namespace cm {
    struct IChannel: public virtual IReadOnlyChannel, public virtual IWriteOnlyChannel {
        
        virtual ~IChannel() {}
    };
}