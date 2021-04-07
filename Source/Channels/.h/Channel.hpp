#pragma once
#include <iostream>
#include "IChannel.hpp"
#include "ReadOnlyChannel.hpp"
#include "WriteOnlyChannel.hpp"

namespace cm {
    struct Channel: public IChannel, public ReadOnlyChannel, public WriteOnlyChannel {
        
        virtual ~Channel() {}
    };
}