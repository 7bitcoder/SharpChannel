#pragma once
#include <memory>
#include <functional>
#include "IUnsubscribable.hpp"
#include "Observers/ICompleteObserver.hpp"
namespace cm
{
    using OnCompleted = std::function<void(void)>;
    
    struct ICompleteObservable
    {
        using Ptr = std::shared_ptr<ICompleteObservable>;

        virtual IUnsubscribable::Ptr subscribe(const OnCompleted &onCompleted) = 0;
        virtual IUnsubscribable::Ptr subscribe(ICompleteObserver &observer) = 0;
        
        virtual ~ICompleteObservable() {}
    };
}