#pragma once
#include <mutex>
#include "Settings.hpp"
#include "Channels/INamedPipeChannel.hpp"
#include "Channels/IChannelEventLoop.hpp"

namespace cm
{
    class NamedPipeChannel final : public INamedPipeChannel
    {
    public:
        using Ptr = std::shared_ptr<NamedPipeChannel>;

        static Ptr create(const NamedPipeChannelSettings &settings, IChannelEventLoop *eventLoop = nullptr);

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
        NamedPipeChannel(const NamedPipeChannelSettings &settings, IChannelEventLoop *eventLoop);
        static INamedPipeChannel::Ptr getImplementation(const NamedPipeChannelSettings &settings, IChannelEventLoop *eventLoop = nullptr);
        INamedPipeChannel::Ptr _impl;
    };
}