#pragma once
#include <memory>
#include <functional>
#include "IUnsubscribable.hpp"

namespace cm
{
    using OnCompleted = std::function<void(void)>;
    
    struct ICompleteObservable
    {
        virtual std::unique_ptr<IUnsubscribable> subscribe(const OnCompleted &onCompleted) = 0;

        virtual ~ICompleteObservable() {}
    };
}