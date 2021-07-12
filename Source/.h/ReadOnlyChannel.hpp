#pragma once
#include <iostream>
#include <functional>
#include <map>
#include <mutex>
#include "Channels/IReadOnlyChannel.hpp"
#include "CallbacksHandler.hpp"
#include "Callbacks.hpp"

namespace cm
{
    class ReadOnlyChannel : public virtual IReadOnlyChannel
    {
    public:
        using Ptr = std::shared_ptr<ReadOnlyChannel>;

        IUnsubscribable::Ptr subscribe(const OnMessageReceived &onMessageReceived) final;
        IUnsubscribable::Ptr subscribe(const OnDataReceived &onDataReceived) final;
        IUnsubscribable::Ptr subscribe(const OnCompleted &onCompleted) final;
        IUnsubscribable::Ptr subscribe(const OnError &onError) final;

        IUnsubscribable::Ptr subscribe(const OnCompleted &onCompleted, const OnError &onError) final;

        IUnsubscribable::Ptr subscribe(const OnMessageReceived &onMessageReceived, const OnCompleted &onCompleted, const OnError &onError) final;
        IUnsubscribable::Ptr subscribe(const OnDataReceived &onDataReceived, const OnCompleted &OnCompleted, const OnError &onError) final;

        IUnsubscribable::Ptr subscribe(IDataObserver &observer) final;
        IUnsubscribable::Ptr subscribe(IErrorObserver &observer) final;
        IUnsubscribable::Ptr subscribe(ICompleteObserver &observer) final;
        IUnsubscribable::Ptr subscribe(IMessageObserver &observer) final;

        IUnsubscribable::Ptr subscribe(IChannelDataObserver &observer) final;
        IUnsubscribable::Ptr subscribe(IChannelMessageObserver &observer) final;

        void setChannelEventLoop(IChannelEventLoop *eventLoop) final;

    protected:
        void completeAll() const;
        void nextAll(const std::string &msg) const;
        void nextAll(const std::vector<char> &data) const;
        void errorAll(const std::exception &error) const;

    private:
        void nextAllImpl(const std::string &msg) const;
        void nextAllImpl(const std::vector<char> &data) const;


        CallbacksHandler<void, const std::string&> _messageCallbacks; 
        CallbacksHandler<void, const std::vector<char> &> _dataCallbacks; 
        CallbacksHandler<void, const std::exception &> _errorCallbacks;
        CallbacksHandler<void> _completeCallbacks; 

        IChannelEventLoop *_eventLoop = nullptr;
    };
}