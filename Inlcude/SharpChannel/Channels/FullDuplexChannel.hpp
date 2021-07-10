#pragma once
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include "Channels/ReadOnlyChannel.hpp"
#include "Channels/WriteOnlyChannel.hpp"

namespace cm
{
    class FullDuplexChannel : public WriteOnlyChannel, public ReadOnlyChannel
    {
    public:
        using Ptr = std::shared_ptr<FullDuplexChannel>;
    };
}