#pragma once
#include <iostream>
#include <functional>
#include <map>
#include "Control.hpp"
#include "IProcessComunicator.hpp"
#include "IObserver.hpp"
#include "IUnsubscribable.hpp"

namespace pm {

    using Next = std::function<Control(const std::string&)>;
    using Complete = std::function<void(void)>;
   
    struct IReadOnlySubject: public virtual IProcessComunicator {

        virtual std::shared_ptr<IUnsubscribable> subscribe(const Next& callback) = 0;
        virtual std::shared_ptr<IUnsubscribable> subscribe(const Next& callback, const Complete& complete) = 0;
        virtual std::shared_ptr<IUnsubscribable> subscribe(IObserver& observer) = 0;
            
        virtual ~IReadOnlySubject() {}
    };
}