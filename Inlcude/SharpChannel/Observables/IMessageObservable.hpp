#pragma once
#include <memory>
#include <functional>
#include "IUnsubscribable.hpp"
#include "Observers/IMessageObserver.hpp"

namespace cm
{
    using OnMessageReceived = std::function<void(const std::string &)>;

    struct IMessageObservable
    {
        using Ptr = std::shared_ptr<IMessageObservable>;

        virtual IUnsubscribable::Ptr subscribe(const OnMessageReceived &onMessageReceived) = 0;
        virtual IUnsubscribable::Ptr subscribe(IMessageObserver &observer) = 0;

        virtual ~IMessageObservable() {}
    };
}