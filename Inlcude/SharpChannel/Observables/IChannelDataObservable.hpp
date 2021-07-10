#pragma once
#include "Observables/ICompleteObservable.hpp"
#include "Observables/IErrorObservable.hpp"
#include "Observables/IDataObservable.hpp"

namespace cm
{
    struct IChannelDataObservable : public ICompleteObservable, public IErrorObservable, public IDataObservable
    {
        using Ptr = std::shared_ptr<IChannelDataObservable>;

        ~IChannelDataObservable() {}
    };
}