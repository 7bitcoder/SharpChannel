#pragma once
#include "ICompleteObserver.hpp"
#include "IErrorObserver.hpp"
#include "IDataObserver.hpp"

namespace cm
{

    struct IChannelDataObserver : public ICompleteObserver, public IErrorObserver, public IDataObserver
    {
        ~IChannelDataObserver() {}
    };
}