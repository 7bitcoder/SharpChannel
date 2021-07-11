#include <utility>
#include <functional>
#include "CommandOutputChannel.hpp"

namespace cm
{
    CommandOutputChannel::CommandOutputChannel(const RunCommandSettings &settings, IChannelEventLoop *eventLoop)
    {
        _impl = CommandOutputChannel::getImplementation(settings, eventLoop);
    }

    CommandOutputChannel::Ptr CommandOutputChannel::create(const RunCommandSettings &settings, IChannelEventLoop *eventLoop)
    {
        return CommandOutputChannel::Ptr(new CommandOutputChannel(settings, eventLoop));
    }

    IUnsubscribable::Ptr CommandOutputChannel::subscribe(const OnMessageReceived &onMessageReceived)
    {
        return _impl->subscribe(onMessageReceived);
    }

    IUnsubscribable::Ptr CommandOutputChannel::subscribe(const OnDataReceived &onDataReceived)
    {
        return _impl->subscribe(onDataReceived);
    }

    IUnsubscribable::Ptr CommandOutputChannel::subscribe(const OnCompleted &onCompleted)
    {
        return _impl->subscribe(onCompleted);
    }

    IUnsubscribable::Ptr CommandOutputChannel::subscribe(const OnError &onError)
    {
        return _impl->subscribe(onError);
    }

    IUnsubscribable::Ptr CommandOutputChannel::subscribe(const OnCompleted &onCompleted, const OnError &onError)
    {
        return _impl->subscribe(onCompleted, onError);
    }

    IUnsubscribable::Ptr CommandOutputChannel::subscribe(const OnMessageReceived &onMessageReceived, const OnCompleted &onCompleted, const OnError &onError)
    {
        return _impl->subscribe(onMessageReceived, onCompleted, onError);
    }

    IUnsubscribable::Ptr CommandOutputChannel::subscribe(const OnDataReceived &onDataReceived, const OnCompleted &onCompleted, const OnError &onError)
    {
        return _impl->subscribe(onDataReceived, onCompleted, onError);
    }

    IUnsubscribable::Ptr CommandOutputChannel::subscribe(IMessageObserver &observer)
    {
        return _impl->subscribe(observer);
    }

    IUnsubscribable::Ptr CommandOutputChannel::subscribe(IDataObserver &observer)
    {
        return _impl->subscribe(observer);
    }

    IUnsubscribable::Ptr CommandOutputChannel::subscribe(IErrorObserver &observer)
    {
        return _impl->subscribe(observer);
    }

    IUnsubscribable::Ptr CommandOutputChannel::subscribe(ICompleteObserver &observer)
    {
        return _impl->subscribe(observer);
    }

    IUnsubscribable::Ptr CommandOutputChannel::subscribe(IChannelDataObserver &observer)
    {
        return _impl->subscribe(observer);
    }

    IUnsubscribable::Ptr CommandOutputChannel::subscribe(IChannelMessageObserver &observer)
    {
        return _impl->subscribe(observer);
    }

    void CommandOutputChannel::setChannelEventLoop(IChannelEventLoop *eventLoop)
    {
        _impl->setChannelEventLoop(eventLoop);
    }

    void CommandOutputChannel::run()
    {
        return _impl->run();
    }
    
    void CommandOutputChannel::finish()
    {
        return _impl->finish();
    }
}
