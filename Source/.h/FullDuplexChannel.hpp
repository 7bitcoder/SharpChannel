#pragma once
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include "ReadOnlyChannel.hpp"
#include "WriteOnlyChannel.hpp"

namespace cm
{
    struct FullDuplexChannel : public WriteOnlyChannel, public ReadOnlyChannel
    {
        using Ptr = std::shared_ptr<FullDuplexChannel>;
    };
}