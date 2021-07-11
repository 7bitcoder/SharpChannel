#pragma once
#include "Settings.hpp"
#include "Channels/ICommandOutputChannel.hpp"

namespace cm
{
    class CommandOutputChannel : public ICommandOutputChannel
    {
    public:
        using Ptr = std::shared_ptr<CommandOutputChannel>;

        static Ptr create(const RunCommandSettings &settings, IChannelEventLoop *eventLoop = nullptr);

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
        CommandOutputChannel(const RunCommandSettings &settings, IChannelEventLoop *eventLoop);
        static ICommandOutputChannel::Ptr getImplementation(const RunCommandSettings &settings, IChannelEventLoop *eventLoop = nullptr);
        ICommandOutputChannel::Ptr _impl;
    };
}