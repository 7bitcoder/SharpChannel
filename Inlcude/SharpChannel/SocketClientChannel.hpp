#pragma once
#include <mutex>
#include "Settings.hpp"
#include "Channels/ISocketClientChannel.hpp"
#include "Channels/IChannelEventLoop.hpp"

namespace cm
{
    class SocketClientChannel final : public ISocketClientChannel
    {
    public:
        using Ptr = std::shared_ptr<SocketClientChannel>;

        static Ptr create(const SocketClientSettings &settings, IChannelEventLoop *eventLoop = nullptr);

        IUnsubscribable::Ptr subscribe(const OnMessageReceived &onMessageReceived) final;
        IUnsubscribable::Ptr subscribe(const OnDataReceived &onDataReceived) final;
        IUnsubscribable::Ptr subscribe(const OnCompleted &onCompleted) final;
        IUnsubscribable::Ptr subscribe(const OnError &onError) final;

        IUnsubscribable::Ptr subscribe(const OnCompleted &onCompleted, const OnError &onError = OnError()) final;

        IUnsubscribable::Ptr subscribe(const OnMessageReceived &onMessageReceived, const OnCompleted &onCompleted = OnCompleted(), const OnError &onError = OnError()) final;
        IUnsubscribable::Ptr subscribe(const OnDataReceived &onDataReceived, const OnCompleted &onCompleted = OnCompleted(), const OnError &onError = OnError()) final;

        IUnsubscribable::Ptr subscribe(IDataObserver &observer) final;
        IUnsubscribable::Ptr subscribe(IErrorObserver &observer) final;
        IUnsubscribable::Ptr subscribe(ICompleteObserver &observer) final;
        IUnsubscribable::Ptr subscribe(IMessageObserver &observer) final;

        IUnsubscribable::Ptr subscribe(IChannelDataObserver &observer) final;
        IUnsubscribable::Ptr subscribe(IChannelMessageObserver &observer) final;

        void error(const std::exception &e) final;
        void complete() final;
        bool next(const std::vector<char> &data) final;
        bool next(const std::string &message) final;

        void setChannelEventLoop(IChannelEventLoop *eventLoop) final;

        void run() final;
        void finish() final;

    private:
        SocketClientChannel(const SocketClientSettings &settings, IChannelEventLoop *eventLoop);
        static ISocketClientChannel::Ptr getImplementation(const SocketClientSettings &settings, IChannelEventLoop *eventLoop = nullptr);
        ISocketClientChannel::Ptr _impl;
    };
}