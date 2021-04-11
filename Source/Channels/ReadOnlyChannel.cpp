#include <utility>
#include <functional>
#include "ReadOnlyChannel.hpp"
#include "IMessageObserver.hpp"
#include "IDataObserver.hpp"

namespace cm
{

    std::unique_ptr<IUnsubscribable> ReadOnlyChannel::subscribe(const OnCompleted &onCompleted, const OnError &onError)
    {
        return _callbacksMap.insert(OnMessageReceived(), onCompleted, onError);
    }

    std::unique_ptr<IUnsubscribable> ReadOnlyChannel::subscribe(const OnMessageReceived &onMessageReceived, const OnCompleted &onCompleted, const OnError &onError)
    {
        return _callbacksMap.insert(onMessageReceived, onCompleted, onError);
    }

    std::unique_ptr<IUnsubscribable> ReadOnlyChannel::subscribe(const OnDataReceived &onDataReceived, const OnCompleted &onCompleted, const OnError &onError)
    {
        return _callbacksMap.insert(onDataReceived, onCompleted, onError);
    }

    std::unique_ptr<IUnsubscribable> ReadOnlyChannel::subscribe(IMessageObserver &observer)
    {
        OnMessageReceived onMessageReceived = std::bind(&IMessageObserver::onMessageReceived, &observer, std::placeholders::_1);
        OnCompleted onCompleted = std::bind(&IMessageObserver::onCompleted, &observer);
        OnError onError = std::bind(&IMessageObserver::onError, &observer, std::placeholders::_1);
        return _callbacksMap.insert(onMessageReceived, onCompleted, onError);
    }

    std::unique_ptr<IUnsubscribable> ReadOnlyChannel::subscribe(IDataObserver &observer)
    {
        OnDataReceived onDataReceived = std::bind(&IDataObserver::onDataReceived, &observer, std::placeholders::_1);
        OnCompleted onCompleted = std::bind(&IDataObserver::onCompleted, &observer);
        OnError onError = std::bind(&IDataObserver::onError, &observer, std::placeholders::_1);
        return _callbacksMap.insert(onDataReceived, onCompleted, onError);
    }

    std::unique_ptr<IUnsubscribable> ReadOnlyChannel::subscribe(IObserver &observer)
    {
        OnCompleted onCompleted = std::bind(&IObserver::onCompleted, &observer);
        OnError onError = std::bind(&IObserver::onError, &observer, std::placeholders::_1);
        return _callbacksMap.insert(OnDataReceived(), onCompleted, onError);
    }

    void ReadOnlyChannel::nextAll(const std::string &msg)
    {
        if (_eventLoop)
        {
            _eventLoop->postChannelEvent([this, msg]() {
                _callbacksMap.nextAll(msg);
            });
        }
        else
        {
            return _callbacksMap.nextAll(msg);
        }
    }

    void ReadOnlyChannel::nextAll(const std::vector<char> &data)
    {
        if (_eventLoop)
        {
            _eventLoop->postChannelEvent([this, data]() {
                _callbacksMap.nextAll(data);
            });
        }
        else
        {
            return _callbacksMap.nextAll(data);
        }
    }

    void ReadOnlyChannel::completeAll()
    {
        if (_eventLoop)
        {
            _eventLoop->postChannelEvent([this]() {
                _callbacksMap.completeAll();
            });

            // send empty to inform that is finished
            _eventLoop->postChannelEvent(std::function<void(void)>()); 
        }
        else
        {
            return _callbacksMap.completeAll();
        }
    }

    void ReadOnlyChannel::errorAll(const std::exception& error) {
        if (_eventLoop)
        {
            _eventLoop->postChannelEvent([this, error]() {
                _callbacksMap.errorAll(error);
            });
        }
        else
        {
            return _callbacksMap.errorAll(error);
        }
    }

    void ReadOnlyChannel::setChannelEventLoop(IChannelEventLoop *eventLoop)
    {
        _eventLoop = eventLoop;
    }
}