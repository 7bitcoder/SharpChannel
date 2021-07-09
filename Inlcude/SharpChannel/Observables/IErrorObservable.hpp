#pragma once
#include <memory>
#include <functional>
#include "IUnsubscribable.hpp"

namespace cm
{
    using OnError = std::function<void(const std::exception &)>;

    struct IErrorObservable
    {
        virtual std::unique_ptr<IUnsubscribable> subscribe(const OnError &onError) = 0;

        virtual ~IErrorObservable() {}
    };
}