#include <utility>
#include <functional>
#include "Channels/ReadOnlyChannel.hpp"

namespace cm
{

    std::unique_ptr<IUnsubscribable> ReadOnlyChannel::subscribe(const OnMessageReceived &onMessageReceived)
    {
        return _callbacksMap->insert(onMessageReceived, OnCompleted(), OnError(), OnConnected());
    }

    std::unique_ptr<IUnsubscribable> ReadOnlyChannel::subscribe(const OnDataReceived &onDataReceived)
    {
        return _callbacksMap->insert(onDataReceived, OnCompleted(), OnError(), OnConnected());
    }

    std::unique_ptr<IUnsubscribable> ReadOnlyChannel::subscribe(const OnCompleted &onCompleted)
    {
        return _callbacksMap->insert(OnMessageReceived(), onCompleted, OnError(), OnConnected());
    }

    std::unique_ptr<IUnsubscribable> ReadOnlyChannel::subscribe(const OnError &onError)
    {
        return _callbacksMap->insert(OnMessageReceived(), OnCompleted(), onError, OnConnected());
    }

    std::unique_ptr<IUnsubscribable> ReadOnlyChannel::subscribe(const OnCompleted &onCompleted, const OnError &onError)
    {
        return _callbacksMap->insert(OnMessageReceived(), onCompleted, onError, OnConnected());
    }

    std::unique_ptr<IUnsubscribable> ReadOnlyChannel::subscribe(const OnMessageReceived &onMessageReceived, const OnCompleted &onCompleted, const OnError &onError)
    {
        return _callbacksMap->insert(onMessageReceived, onCompleted, onError, OnConnected());
    }

    std::unique_ptr<IUnsubscribable> ReadOnlyChannel::subscribe(const OnDataReceived &onDataReceived, const OnCompleted &onCompleted, const OnError &onError)
    {
        return _callbacksMap->insert(onDataReceived, onCompleted, onError, OnConnected());
    }

    std::unique_ptr<IUnsubscribable> ReadOnlyChannel::subscribe(IMessageObserver &observer)
    {
        OnMessageReceived onMessageReceived = std::bind(&IMessageObserver::next, &observer, std::placeholders::_1);
        return _callbacksMap->insert(onMessageReceived, OnCompleted(), OnError(), OnConnected());
    }

    std::unique_ptr<IUnsubscribable> ReadOnlyChannel::subscribe(IDataObserver &observer)
    {
        OnDataReceived onDataReceived = std::bind(&IDataObserver::next, &observer, std::placeholders::_1);
        return _callbacksMap->insert(onDataReceived, OnCompleted(), OnError(), OnConnected());
    }

    std::unique_ptr<IUnsubscribable> ReadOnlyChannel::subscribe(IErrorObserver &observer)
    {
        OnError onError = std::bind(&IErrorObserver::error, &observer, std::placeholders::_1);
        return _callbacksMap->insert(OnDataReceived(), OnCompleted(), onError, OnConnected());
    }

    std::unique_ptr<IUnsubscribable> ReadOnlyChannel::subscribe(ICompleteObserver &observer)
    {
        OnCompleted onCompleted = std::bind(&ICompleteObserver::complete, &observer);
        return _callbacksMap->insert(OnDataReceived(), onCompleted, OnError(), OnConnected());
    }

    std::unique_ptr<IUnsubscribable> ReadOnlyChannel::subscribe(IChannelDataObserver &observer)
    {
        OnDataReceived onMessageReceived = std::bind(&IDataObserver::next, &observer, std::placeholders::_1);
        OnCompleted onCompleted = std::bind(&ICompleteObserver::complete, &observer);
        OnError onError = std::bind(&IErrorObserver::error, &observer, std::placeholders::_1);
        return _callbacksMap->insert(onMessageReceived, onCompleted, onError, OnConnected());
    }

    std::unique_ptr<IUnsubscribable> ReadOnlyChannel::subscribe(IChannelMessageObserver &observer)
    {
        OnMessageReceived onMessageReceived = std::bind(&IMessageObserver::next, &observer, std::placeholders::_1);
        OnCompleted onCompleted = std::bind(&ICompleteObserver::complete, &observer);
        OnError onError = std::bind(&IErrorObserver::error, &observer, std::placeholders::_1);
        return _callbacksMap->insert(onMessageReceived, onCompleted, onError, OnConnected());
    }

    void ReadOnlyChannel::nextAll(const std::string &msg)
    {
        if (_eventLoop)
        {
            _eventLoop->postChannelEvent([this, msg]()
                                         { _callbacksMap->nextAll(msg); });
        }
        else
        {
            return _callbacksMap->nextAll(msg);
        }
    }

    void ReadOnlyChannel::nextAll(const std::vector<char> &data)
    {
        if (_eventLoop)
        {
            _eventLoop->postChannelEvent([this, data]()
                                         { _callbacksMap->nextAll(data); });
        }
        else
        {
            return _callbacksMap->nextAll(data);
        }
    }

    void ReadOnlyChannel::completeAll()
    {
        if (_eventLoop)
        {
            _eventLoop->postChannelEvent([this]()
                                         { _callbacksMap->completeAll(); });

            // send empty to inform that is finished
            _eventLoop->postChannelEvent(std::function<void(void)>());
        }
        else
        {
            return _callbacksMap->completeAll();
        }
    }

    void ReadOnlyChannel::errorAll(const std::exception &error)
    {
        if (_eventLoop)
        {
            _eventLoop->postChannelEvent([this, error]()
                                         { _callbacksMap->errorAll(error); });
        }
        else
        {
            return _callbacksMap->errorAll(error);
        }
    }

    void ReadOnlyChannel::setChannelEventLoop(IChannelEventLoop *eventLoop)
    {
        _eventLoop = eventLoop;
    }
}