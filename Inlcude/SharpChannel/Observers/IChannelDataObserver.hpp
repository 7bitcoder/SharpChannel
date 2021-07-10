#pragma once
#include "Observers/ICompleteObserver.hpp"
#include "Observers/IErrorObserver.hpp"
#include "Observers/IDataObserver.hpp"

namespace cm
{

    struct IChannelDataObserver : public ICompleteObserver, public IErrorObserver, public IDataObserver
    {
    };
}