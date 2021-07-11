#pragma once
#include "Settings.hpp"
#include "Channels/ICommandChannel.hpp"

namespace cm
{
    class CommandChannel final : ICommandChannel
    {
    public:
        using Ptr = std::shared_ptr<CommandChannel>;

        static CommandChannel::Ptr create(const RunCommandSettings &settings, IChannelEventLoop *eventLoop = nullptr);

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

        void run() final;
        void finish() final;
    private:
        CommandChannel(const RunCommandSettings &settings, IChannelEventLoop *eventLoop);
        static ICommandChannel::Ptr getImplementation(const RunCommandSettings &settings, IChannelEventLoop *eventLoop = nullptr);
        ICommandChannel::Ptr _impl;
    };
}