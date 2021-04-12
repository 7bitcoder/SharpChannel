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
        std::unique_ptr<IUnsubscribable> subscribe(const OnCompleted &onCompleted, const OnError &onError = OnError()) final;

        std::unique_ptr<IUnsubscribable> subscribe(const OnMessageReceived &onMessageReceived, const OnCompleted &onCompleted = OnCompleted(), const OnError &onError = OnError()) final;
        std::unique_ptr<IUnsubscribable> subscribe(const OnDataReceived &onDataReceived, const OnCompleted &onCompleted = OnCompleted(), const OnError &onError = OnError()) final;

        std::unique_ptr<IUnsubscribable> subscribe(IMessageObserver &observer) final;
        std::unique_ptr<IUnsubscribable> subscribe(IDataObserver &observer) final;
        std::unique_ptr<IUnsubscribable> subscribe(IObserver &observer) final;

        void setChannelEventLoop(IChannelEventLoop *eventLoop);
        ~ReadOnlyChannel() {}

    protected:
        void completeAll();
        void nextAll(const std::string &msg);
        void nextAll(const std::vector<char> &data);
        void errorAll(const std::exception& error);

    private:
        CallbacksMap _callbacksMap;
        IChannelEventLoop *_eventLoop = nullptr;
    };
}