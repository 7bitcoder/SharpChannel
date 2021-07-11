#pragma once
#include "IObservables.hpp"
#include "IObservers.hpp"
#include "Channels/IChannelEventLoop.hpp"

namespace cm
{
    using OnConnected = std::function<void(void)>;

    struct IReadOnlyChannel : public IMessageObservable, public ICompleteObservable, public IErrorObservable, public IDataObservable
    {
        using Ptr = std::shared_ptr<IReadOnlyChannel>;

        IUnsubscribable::Ptr subscribe(const OnMessageReceived &onMessageReceived) override = 0;
        IUnsubscribable::Ptr subscribe(const OnDataReceived &onDataReceived) override = 0;
        IUnsubscribable::Ptr subscribe(const OnCompleted &onCompleted) override = 0;
        IUnsubscribable::Ptr subscribe(const OnError &onError) override = 0;

        virtual IUnsubscribable::Ptr subscribe(const OnCompleted &onCompleted, const OnError &onError) = 0;

        virtual IUnsubscribable::Ptr subscribe(const OnMessageReceived &onMessageReceived, const OnCompleted &onCompleted, const OnError &onError) = 0;
        virtual IUnsubscribable::Ptr subscribe(const OnDataReceived &onDataReceived, const OnCompleted &OnCompleted, const OnError &onError) = 0;

        virtual IUnsubscribable::Ptr subscribe(IChannelDataObserver &observer) = 0;
        virtual IUnsubscribable::Ptr subscribe(IChannelMessageObserver &observer) = 0;

        IUnsubscribable::Ptr subscribe(IDataObserver &observer) override = 0;
        IUnsubscribable::Ptr subscribe(IErrorObserver &observer) override = 0;
        IUnsubscribable::Ptr subscribe(ICompleteObserver &observer) override = 0;
        IUnsubscribable::Ptr subscribe(IMessageObserver &observer) override = 0;

        virtual void setChannelEventLoop(IChannelEventLoop *eventLoop) = 0;
    };
}