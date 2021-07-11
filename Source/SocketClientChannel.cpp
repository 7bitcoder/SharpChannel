#include <utility>
#include <functional>
#include "SocketClientChannel.hpp"

namespace cm
{
    SocketClientChannel::SocketClientChannel(const SocketClientSettings &settings, IChannelEventLoop *eventLoop)
    {
        _impl = SocketClientChannel::getImplementation(settings, eventLoop);
    }

    SocketClientChannel::Ptr SocketClientChannel::create(const SocketClientSettings &settings, IChannelEventLoop *eventLoop)
    {
        return SocketClientChannel::Ptr(new SocketClientChannel(settings, eventLoop));
    }

    IUnsubscribable::Ptr SocketClientChannel::subscribe(const OnMessageReceived &onMessageReceived)
    {
        return _impl->subscribe(onMessageReceived);
    }

    IUnsubscribable::Ptr SocketClientChannel::subscribe(const OnDataReceived &onDataReceived)
    {
        return _impl->subscribe(onDataReceived);
    }

    IUnsubscribable::Ptr SocketClientChannel::subscribe(const OnCompleted &onCompleted)
    {
        return _impl->subscribe(onCompleted);
    }

    IUnsubscribable::Ptr SocketClientChannel::subscribe(const OnError &onError)
    {
        return _impl->subscribe(onError);
    }

    IUnsubscribable::Ptr SocketClientChannel::subscribe(const OnCompleted &onCompleted, const OnError &onError)
    {
        return _impl->subscribe(onCompleted, onError);
    }

    IUnsubscribable::Ptr SocketClientChannel::subscribe(const OnMessageReceived &onMessageReceived, const OnCompleted &onCompleted, const OnError &onError)
    {
        return _impl->subscribe(onMessageReceived, onCompleted, onError);
    }

    IUnsubscribable::Ptr SocketClientChannel::subscribe(const OnDataReceived &onDataReceived, const OnCompleted &onCompleted, const OnError &onError)
    {
        return _impl->subscribe(onDataReceived, onCompleted, onError);
    }

    IUnsubscribable::Ptr SocketClientChannel::subscribe(IMessageObserver &observer)
    {
        return _impl->subscribe(observer);
    }

    IUnsubscribable::Ptr SocketClientChannel::subscribe(IDataObserver &observer)
    {
        return _impl->subscribe(observer);
    }

    IUnsubscribable::Ptr SocketClientChannel::subscribe(IErrorObserver &observer)
    {
        return _impl->subscribe(observer);
    }

    IUnsubscribable::Ptr SocketClientChannel::subscribe(ICompleteObserver &observer)
    {
        return _impl->subscribe(observer);
    }

    IUnsubscribable::Ptr SocketClientChannel::subscribe(IChannelDataObserver &observer)
    {
        return _impl->subscribe(observer);
    }

    IUnsubscribable::Ptr SocketClientChannel::subscribe(IChannelMessageObserver &observer)
    {
        return _impl->subscribe(observer);
    }

    void SocketClientChannel::setChannelEventLoop(IChannelEventLoop *eventLoop)
    {
        _impl->setChannelEventLoop(eventLoop);
    }

    void SocketClientChannel::run()
    {
        return _impl->run();
    }

    void SocketClientChannel::finish()
    {
        return _impl->finish();
    }

    void SocketClientChannel::error(const std::exception &e)
    {
        return _impl->error(e);
    }

    void SocketClientChannel::complete()
    {
        return _impl->complete();
    }

    bool SocketClientChannel::next(const std::vector<char> &data)
    {
        return _impl->IDataObserver::next(data);
    }
    bool SocketClientChannel::next(const std::string &message)
    {
        return _impl->IMessageObserver::next(message);
    }
}
