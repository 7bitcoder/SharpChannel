#include <utility>
#include <functional>
#include "NamedPipeChannel.hpp"

namespace cm
{
    NamedPipeChannel::NamedPipeChannel(const NamedPipeChannelSettings &settings, IChannelEventLoop *eventLoop)
    {
        _impl = NamedPipeChannel::getImplementation(settings, eventLoop);
    }

    NamedPipeChannel::Ptr NamedPipeChannel::create(const NamedPipeChannelSettings &settings, IChannelEventLoop *eventLoop)
    {
        return NamedPipeChannel::Ptr(new NamedPipeChannel(settings, eventLoop));
    }

    IUnsubscribable::Ptr NamedPipeChannel::subscribe(const OnMessageReceived &onMessageReceived)
    {
        return _impl->subscribe(onMessageReceived);
    }

    IUnsubscribable::Ptr NamedPipeChannel::subscribe(const OnDataReceived &onDataReceived)
    {
        return _impl->subscribe(onDataReceived);
    }

    IUnsubscribable::Ptr NamedPipeChannel::subscribe(const OnCompleted &onCompleted)
    {
        return _impl->subscribe(onCompleted);
    }

    IUnsubscribable::Ptr NamedPipeChannel::subscribe(const OnError &onError)
    {
        return _impl->subscribe(onError);
    }

    IUnsubscribable::Ptr NamedPipeChannel::subscribe(const OnCompleted &onCompleted, const OnError &onError)
    {
        return _impl->subscribe(onCompleted, onError);
    }

    IUnsubscribable::Ptr NamedPipeChannel::subscribe(const OnMessageReceived &onMessageReceived, const OnCompleted &onCompleted, const OnError &onError)
    {
        return _impl->subscribe(onMessageReceived, onCompleted, onError);
    }

    IUnsubscribable::Ptr NamedPipeChannel::subscribe(const OnDataReceived &onDataReceived, const OnCompleted &onCompleted, const OnError &onError)
    {
        return _impl->subscribe(onDataReceived, onCompleted, onError);
    }

    IUnsubscribable::Ptr NamedPipeChannel::subscribe(IMessageObserver &observer)
    {
        return _impl->subscribe(observer);
    }

    IUnsubscribable::Ptr NamedPipeChannel::subscribe(IDataObserver &observer)
    {
        return _impl->subscribe(observer);
    }

    IUnsubscribable::Ptr NamedPipeChannel::subscribe(IErrorObserver &observer)
    {
        return _impl->subscribe(observer);
    }

    IUnsubscribable::Ptr NamedPipeChannel::subscribe(ICompleteObserver &observer)
    {
        return _impl->subscribe(observer);
    }

    IUnsubscribable::Ptr NamedPipeChannel::subscribe(IChannelDataObserver &observer)
    {
        return _impl->subscribe(observer);
    }

    IUnsubscribable::Ptr NamedPipeChannel::subscribe(IChannelMessageObserver &observer)
    {
        return _impl->subscribe(observer);
    }

    void NamedPipeChannel::setChannelEventLoop(IChannelEventLoop *eventLoop)
    {
        _impl->setChannelEventLoop(eventLoop);
    }

    void NamedPipeChannel::run()
    {
        return _impl->run();
    }

    void NamedPipeChannel::finish()
    {
        return _impl->finish();
    }

    void NamedPipeChannel::error(const std::exception &e)
    {
        return _impl->error(e);
    }

    void NamedPipeChannel::complete()
    {
        return _impl->complete();
    }

    bool NamedPipeChannel::next(const std::vector<char> &data)
    {
        return _impl->IDataObserver::next(data);
    }
    bool NamedPipeChannel::next(const std::string &message)
    {
        return _impl->IMessageObserver::next(message);
    }
}
