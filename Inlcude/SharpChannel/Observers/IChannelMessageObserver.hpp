#pragma once
#include "Observers/ICompleteObserver.hpp"
#include "Observers/IErrorObserver.hpp"
#include "Observers/IMessageObserver.hpp"

namespace cm
{

    struct IChannelMessageObserver : public ICompleteObserver, public IErrorObserver, public IMessageObserver
    {
        ~IChannelMessageObserver() {}
    };
}