#pragma once
#include "ICompleteObserver.hpp"
#include "IErrorObserver.hpp"
#include "IMessageObserver.hpp"
#include "IDataObserver.hpp"

namespace cm
{

    struct IChannelObserver : public ICompleteObserver, public IErrorObserver, public IMessageObserver, public IDataObserver
    {
        ~IChannelObserver() {}
    };
}