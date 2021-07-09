#pragma once
#include "IObservables.hpp"
#include "IObservers.hpp"

namespace cm
{
    using OnConnected = std::function<void(void)>;

    struct IReadOnlyChannel: public IMessageObservable, public ICompleteObservable, public IErrorObservable, public IDataObservable
    {

        virtual std::unique_ptr<IUnsubscribable> subscribe(const OnCompleted &onCompleted, const OnError &onError) = 0;

        virtual std::unique_ptr<IUnsubscribable> subscribe(const OnMessageReceived &onMessageReceived, const OnCompleted &onCompleted, const OnError &onError) = 0;
        virtual std::unique_ptr<IUnsubscribable> subscribe(const OnDataReceived &onDataReceived, const OnCompleted &OnCompleted, const OnError &onError) = 0;

        virtual std::unique_ptr<IUnsubscribable> subscribe(IDataObserver &observer) = 0;
        virtual std::unique_ptr<IUnsubscribable> subscribe(IErrorObserver &observer) = 0;
        virtual std::unique_ptr<IUnsubscribable> subscribe(ICompleteObserver &observer) = 0;
        virtual std::unique_ptr<IUnsubscribable> subscribe(IMessageObserver &observer) = 0;

        virtual std::unique_ptr<IUnsubscribable> subscribe(IChannelDataObserver &observer) = 0;
        virtual std::unique_ptr<IUnsubscribable> subscribe(IChannelMessageObserver &observer) = 0;

        virtual ~IReadOnlyChannel() {}
    };
}