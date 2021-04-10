#pragma once
#include <iostream>
#include <functional>
#include "IMessageObserver.hpp"
#include "IDataObserver.hpp"
#include "IRunnable.hpp"
#include "IUnsubscribable.hpp"
#include "Control.hpp"

namespace cm {

    using OnMessageReceived = std::function<Control(const std::string&)>;
    using OnDataReceived = std::function<Control(const std::vector<char>&)>;
    using onCompleted = std::function<void(void)>;
   
    struct IReadOnlyChannel: public virtual IRunnable {

        virtual std::shared_ptr<IUnsubscribable> subscribe(const onCompleted& onCompleted) = 0;

        virtual std::shared_ptr<IUnsubscribable> subscribe(const OnMessageReceived& onMessageReceived) = 0;
        virtual std::shared_ptr<IUnsubscribable> subscribe(const OnDataReceived& onDataReceived) = 0;

        virtual std::shared_ptr<IUnsubscribable> subscribe(const OnMessageReceived& onMessageReceived, const onCompleted& onCompleted) = 0;
        virtual std::shared_ptr<IUnsubscribable> subscribe(const OnDataReceived& onDataReceived, const onCompleted& onCompleted) = 0;

        virtual std::shared_ptr<IUnsubscribable> subscribe(IMessageObserver& observer) = 0;
        virtual std::shared_ptr<IUnsubscribable> subscribe(IDataObserver& observer) = 0;
        virtual std::shared_ptr<IUnsubscribable> subscribe(IObserver& observer) = 0;
            
        virtual ~IReadOnlyChannel() {}
    };
}