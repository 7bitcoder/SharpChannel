#pragma once
#include "IObservables.hpp"
#include "IObservers.hpp"

namespace cm
{
    using OnConnected = std::function<void(void)>;

    struct IReadOnlyChannel: public IMessageObservable, public ICompleteObservable, public IErrorObservable, public IDataObservable
    {
        using Ptr = std::shared_ptr<IReadOnlyChannel>;

        virtual IUnsubscribable::Ptr subscribe(const OnCompleted &onCompleted, const OnError &onError) = 0;

        virtual IUnsubscribable::Ptr subscribe(const OnMessageReceived &onMessageReceived, const OnCompleted &onCompleted, const OnError &onError) = 0;
        virtual IUnsubscribable::Ptr subscribe(const OnDataReceived &onDataReceived, const OnCompleted &OnCompleted, const OnError &onError) = 0;

        virtual IUnsubscribable::Ptr subscribe(IChannelDataObserver &observer) = 0;
        virtual IUnsubscribable::Ptr subscribe(IChannelMessageObserver &observer) = 0;

        virtual ~IReadOnlyChannel() {}
    };
}