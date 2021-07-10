#pragma once
#include <memory>
#include <functional>
#include "IUnsubscribable.hpp"
#include "Observers/IErrorObserver.hpp"

namespace cm
{
    using OnError = std::function<void(const std::exception &)>;

    struct IErrorObservable
    {
        using Ptr = std::shared_ptr<IErrorObservable>;

        virtual IUnsubscribable::Ptr subscribe(const OnError &onError) = 0;
        virtual IUnsubscribable::Ptr subscribe(IErrorObserver &observer) = 0;

        virtual ~IErrorObservable() {}
    };
}