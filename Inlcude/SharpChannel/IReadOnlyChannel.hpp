#pragma once
#include <iostream>
#include <functional>
#include "IMessageObserver.hpp"
#include "IDataObserver.hpp"
#include "IRunnable.hpp"
#include "IUnsubscribable.hpp"


namespace cm
{

    using OnMessageReceived = std::function<void(const std::string &)>;
    using OnDataReceived = std::function<void(const std::vector<char> &)>;
    using OnCompleted = std::function<void(void)>;
    using OnError = std::function<void(const std::exception &)>;

    struct IReadOnlyChannel : public virtual IRunnable
    {

        virtual std::unique_ptr<IUnsubscribable> subscribe(const OnCompleted &onCompleted, const OnError &onError = OnError()) = 0;

        virtual std::unique_ptr<IUnsubscribable> subscribe(const OnMessageReceived &onMessageReceived, const OnCompleted &onCompleted = OnCompleted(), const OnError &onError = OnError()) = 0;
        virtual std::unique_ptr<IUnsubscribable> subscribe(const OnDataReceived &onDataReceived, const OnCompleted &onCompleted = OnCompleted(), const OnError &onError = OnError()) = 0;

        virtual std::unique_ptr<IUnsubscribable> subscribe(IMessageObserver &observer) = 0;
        virtual std::unique_ptr<IUnsubscribable> subscribe(IDataObserver &observer) = 0;
        virtual std::unique_ptr<IUnsubscribable> subscribe(IObserver &observer) = 0;

        virtual ~IReadOnlyChannel() {}
    };
}