#pragma once
#include "Observables/ICompleteObservable.hpp"
#include "Observables/IErrorObservable.hpp"
#include "Observables/IMessageObservable.hpp"

namespace cm
{
    struct IChannelMessageObservable : public ICompleteObservable, public IErrorObservable, public IMessageObservable
    {
        using Ptr = std::shared_ptr<IChannelMessageObservable>;
    };
}