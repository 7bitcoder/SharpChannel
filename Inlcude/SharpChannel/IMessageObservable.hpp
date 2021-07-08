#pragma once
#include <memory>
#include <functional>
#include"IUnsubscribable.hpp"

namespace cm
{
    using OnMessageReceived = std::function<void(const std::string &)>;

    struct IMessageObservable
    {
        virtual std::unique_ptr<IUnsubscribable> subscribe(const OnMessageReceived &onMessageReceived) = 0;

        virtual ~IMessageObservable() {}
    };
}