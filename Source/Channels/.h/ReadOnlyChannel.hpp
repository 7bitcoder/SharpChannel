#pragma once
#include <iostream>
#include <functional>
#include <map>
#include <mutex>
#include "IReadOnlyChannel.hpp"
#include "IChannelEventLoop.hpp"
#include "CallbacksMap.hpp"

namespace cm
{
    class ReadOnlyChannel : public virtual IReadOnlyChannel
    {
    public:
        std::shared_ptr<IUnsubscribable> subscribe(const onCompleted &onCompleted) final;

        std::shared_ptr<IUnsubscribable> subscribe(const OnMessageReceived &onMessageReceived) final;
        std::shared_ptr<IUnsubscribable> subscribe(const OnDataReceived &onDataReceived) final;

        std::shared_ptr<IUnsubscribable> subscribe(const OnMessageReceived &onMessageReceived, const onCompleted &onCompleted) final;
        std::shared_ptr<IUnsubscribable> subscribe(const OnDataReceived &onDataReceived, const onCompleted &onCompleted) final;

        std::shared_ptr<IUnsubscribable> subscribe(IMessageObserver &observer) final;
        std::shared_ptr<IUnsubscribable> subscribe(IDataObserver &observer) final;
        std::shared_ptr<IUnsubscribable> subscribe(IObserver &observer) final;

        void setChannelEventLoop(IChannelEventLoop *eventLoop);
        virtual ~ReadOnlyChannel() {}

    protected:
        void completeAll();
        void nextAll(const std::string &msg);
        void nextAll(const std::vector<char> &data);

    private:
        CallbacksMap _callbacksMap;
        IChannelEventLoop *_eventLoop = nullptr;
    };
}