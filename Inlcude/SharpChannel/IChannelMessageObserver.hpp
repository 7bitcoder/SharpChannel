#pragma once
#include "ICompleteObserver.hpp"
#include "IErrorObserver.hpp"
#include "IMessageObserver.hpp"

namespace cm
{

    struct IChannelMessageObserver : public ICompleteObserver, public IErrorObserver, public IMessageObserver
    {
        ~IChannelMessageObserver() {}
    };
}