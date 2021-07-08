#pragma once
#include <iostream>
#include <functional>
#include <memory>
#include "IMessageObservable.hpp"
#include "IDataObservable.hpp"
#include "IErrorObservable.hpp"
#include "ICompleteObservable.hpp"
#include "IUnsubscribable.hpp"
#include "IChannelObserver.hpp"

namespace cm
{
    using OnConnected = std::function<void(void)>;

    struct IChannelObservable: public IMessageObservable, public ICompleteObservable, public IErrorObservable, public IDataObservable
    {

        virtual std::unique_ptr<IUnsubscribable> subscribe(const OnCompleted &onCompleted, const OnError &onError) = 0;

        virtual std::unique_ptr<IUnsubscribable> subscribe(const OnMessageReceived &onMessageReceived, const OnCompleted &onCompleted, const OnError &onError) = 0;
        virtual std::unique_ptr<IUnsubscribable> subscribe(const OnDataReceived &onDataReceived, const OnCompleted &OnCompleted, const OnError &onError) = 0;

        virtual std::unique_ptr<IUnsubscribable> subscribe(IDataObserver &observer) = 0;
        virtual std::unique_ptr<IUnsubscribable> subscribe(IErrorObserver &observer) = 0;
        virtual std::unique_ptr<IUnsubscribable> subscribe(ICompleteObserver &observer) = 0;
        virtual std::unique_ptr<IUnsubscribable> subscribe(IMessageObserver &observer) = 0;

        virtual std::unique_ptr<IUnsubscribable> subscribe(IChannelObserver &observer) = 0;

        virtual ~IChannelObservable() {}
    };
}