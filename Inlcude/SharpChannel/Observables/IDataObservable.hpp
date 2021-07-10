#pragma once
#include <memory>
#include <vector>
#include <functional>
#include "IUnsubscribable.hpp"
#include "Observers/IDataObserver.hpp"

namespace cm
{
    using OnDataReceived = std::function<void(const std::vector<char> &)>;

    struct IDataObservable
    {
        using Ptr = std::shared_ptr<IDataObservable>;

        virtual IUnsubscribable::Ptr subscribe(const OnDataReceived &onDataReceived) = 0;
        virtual IUnsubscribable::Ptr subscribe(IDataObserver &observer) = 0;

        virtual ~IDataObservable() {}
    };
}