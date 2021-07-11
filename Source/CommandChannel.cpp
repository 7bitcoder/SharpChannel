#include <utility>
#include <functional>
#include "CommandChannel.hpp"

namespace cm
{
    CommandChannel::CommandChannel(const RunCommandSettings &settings, IChannelEventLoop *eventLoop)
    {
        _impl = CommandChannel::getImplementation(settings, eventLoop);
    }

    CommandChannel::Ptr CommandChannel::create(const RunCommandSettings &settings, IChannelEventLoop *eventLoop)
    {
        return CommandChannel::Ptr(new CommandChannel(settings, eventLoop));
    }

    IUnsubscribable::Ptr CommandChannel::subscribe(const OnMessageReceived &onMessageReceived)
    {
        return IMessageObservable::Ptr(_impl)->subscribe(onMessageReceived);
    }

    IUnsubscribable::Ptr CommandChannel::subscribe(const OnDataReceived &onDataReceived)
    {
        return IDataObservable::Ptr(_impl)->subscribe(onDataReceived);
    }

    IUnsubscribable::Ptr CommandChannel::subscribe(const OnCompleted &onCompleted)
    {
        return ICompleteObservable::Ptr(_impl)->subscribe(onCompleted);
    }

    IUnsubscribable::Ptr CommandChannel::subscribe(const OnError &onError)
    {
        return IErrorObservable::Ptr(_impl)->subscribe(onError);
    }

    IUnsubscribable::Ptr CommandChannel::subscribe(const OnCompleted &onCompleted, const OnError &onError)
    {
        return _impl->subscribe(onCompleted, onError);
    }

    IUnsubscribable::Ptr CommandChannel::subscribe(const OnMessageReceived &onMessageReceived, const OnCompleted &onCompleted, const OnError &onError)
    {
        return _impl->subscribe(onMessageReceived, onCompleted, onError);
    }

    IUnsubscribable::Ptr CommandChannel::subscribe(const OnDataReceived &onDataReceived, const OnCompleted &onCompleted, const OnError &onError)
    {
        return _impl->subscribe(onDataReceived, onCompleted, onError);
    }

    IUnsubscribable::Ptr CommandChannel::subscribe(IMessageObserver &observer)
    {
        return IMessageObservable::Ptr(_impl)->subscribe(observer);
    }

    IUnsubscribable::Ptr CommandChannel::subscribe(IDataObserver &observer)
    {
        return IDataObservable::Ptr(_impl)->subscribe(observer);
    }

    IUnsubscribable::Ptr CommandChannel::subscribe(IErrorObserver &observer)
    {
        return IErrorObservable::Ptr(_impl)->subscribe(observer);
    }

    IUnsubscribable::Ptr CommandChannel::subscribe(ICompleteObserver &observer)
    {
        return ICompleteObservable::Ptr(_impl)->subscribe(observer);
    }

    IUnsubscribable::Ptr CommandChannel::subscribe(IChannelDataObserver &observer)
    {
        return _impl->subscribe(observer);
    }

    IUnsubscribable::Ptr CommandChannel::subscribe(IChannelMessageObserver &observer)
    {
        return _impl->subscribe(observer);
    }

    void CommandChannel::setChannelEventLoop(IChannelEventLoop *eventLoop)
    {
        _impl->setChannelEventLoop(eventLoop);
    }

    void CommandChannel::run()
    {
        return _impl->run();
    }
    
    void CommandChannel::finish()
    {
        return _impl->finish();
    }
}
