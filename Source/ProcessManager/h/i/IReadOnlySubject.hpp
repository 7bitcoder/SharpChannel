#pragma once
#include <iostream>
#include <functional>
#include <map>
#include "Control.hpp"
#include "IProcessComunicator.hpp"
#include "IObserver.hpp"
#include "IUnsubscribable.hpp"

namespace pm {

    using OnReceive = std::function<Control(const std::string&)>;
    using OnComplete = std::function<void(void)>;
   
    struct IReadOnlySubject: public virtual IProcessComunicator {

        virtual std::shared_ptr<IUnsubscribable> subscribe(const OnReceive& onReceive) = 0;
        virtual std::shared_ptr<IUnsubscribable> subscribe(const OnReceive& onReceive, const OnComplete& onComplete) = 0;
        virtual std::shared_ptr<IUnsubscribable> subscribe(IObserver& observer) = 0;
            
        virtual ~IReadOnlySubject() {}
    };
}