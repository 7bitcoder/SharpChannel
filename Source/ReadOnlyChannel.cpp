#include <utility>
#include <functional>
#include "ReadOnlyChannel.hpp"

namespace cm
{

    IUnsubscribable::Ptr ReadOnlyChannel::subscribe(const OnMessageReceived &onMessageReceived)
    {
        return _messageHandler.add(onMessageReceived);
    }

    IUnsubscribable::Ptr ReadOnlyChannel::subscribe(const OnDataReceived &onDataReceived)
    {
        return _dataHandler.add(onDataReceived);
    }

    IUnsubscribable::Ptr ReadOnlyChannel::subscribe(const OnCompleted &onCompleted)
    {
        return _completeHandler.add(onCompleted);
    }

    IUnsubscribable::Ptr ReadOnlyChannel::subscribe(const OnError &onError)
    {
        return _errorHandler.add(onError);
    }

    IUnsubscribable::Ptr ReadOnlyChannel::subscribe(const OnCompleted &onCompleted, const OnError &onError)
    {
        return Unsubscriber::Combine({subscribe(onCompleted), subscribe(onError)});
    }

    IUnsubscribable::Ptr ReadOnlyChannel::subscribe(const OnMessageReceived &onMessageReceived, const OnCompleted &onCompleted, const OnError &onError)
    {
        return Unsubscriber::Combine({subscribe(onMessageReceived), subscribe(onCompleted), subscribe(onError)});
    }

    IUnsubscribable::Ptr ReadOnlyChannel::subscribe(const OnDataReceived &onDataReceived, const OnCompleted &onCompleted, const OnError &onError)
    {
        return Unsubscriber::Combine({subscribe(onDataReceived), subscribe(onCompleted), subscribe(onError)});
    }

    IUnsubscribable::Ptr ReadOnlyChannel::subscribe(IMessageObserver &observer)
    {
        OnMessageReceived onMessageReceived = std::bind(&IMessageObserver::next, &observer, std::placeholders::_1);
        return subscribe(onMessageReceived);
    }

    IUnsubscribable::Ptr ReadOnlyChannel::subscribe(IDataObserver &observer)
    {
        OnDataReceived onDataReceived = std::bind(&IDataObserver::next, &observer, std::placeholders::_1);
        return subscribe(onDataReceived);
    }

    IUnsubscribable::Ptr ReadOnlyChannel::subscribe(IErrorObserver &observer)
    {
        OnError onError = std::bind(&IErrorObserver::error, &observer, std::placeholders::_1);
        return subscribe(onError);
    }

    IUnsubscribable::Ptr ReadOnlyChannel::subscribe(ICompleteObserver &observer)
    {
        OnCompleted onCompleted = std::bind(&ICompleteObserver::complete, &observer);
        return subscribe(onCompleted);
    }

    IUnsubscribable::Ptr ReadOnlyChannel::subscribe(IChannelDataObserver &observer)
    {
        OnDataReceived onMessageReceived = std::bind(&IDataObserver::next, &observer, std::placeholders::_1);
        OnCompleted onCompleted = std::bind(&ICompleteObserver::complete, &observer);
        OnError onError = std::bind(&IErrorObserver::error, &observer, std::placeholders::_1);
        return subscribe(onMessageReceived, onCompleted, onError);
    }

    IUnsubscribable::Ptr ReadOnlyChannel::subscribe(IChannelMessageObserver &observer)
    {
        OnMessageReceived onMessageReceived = std::bind(&IMessageObserver::next, &observer, std::placeholders::_1);
        OnCompleted onCompleted = std::bind(&ICompleteObserver::complete, &observer);
        OnError onError = std::bind(&IErrorObserver::error, &observer, std::placeholders::_1);
        return subscribe(onMessageReceived, onCompleted, onError);
    }

    void ReadOnlyChannel::nextAll(const std::string &msg) const
    {
        if (_eventLoop)
        {
            _eventLoop->postChannelEvent([this, msg]()
                                         { nextAllImpl(msg); });
        }
        else
        {
            return nextAllImpl(msg);
        }
    }

    void ReadOnlyChannel::nextAll(const std::vector<char> &data) const
    {
        if (_eventLoop)
        {
            _eventLoop->postChannelEvent([this, data]()
                                         { nextAllImpl(data); });
        }
        else
        {
            return nextAllImpl(data);
        }
    }

    void ReadOnlyChannel::completeAll() const
    {
        if (_eventLoop)
        {
            _eventLoop->postChannelEvent([this]()
                                         { _completeHandler.callAll(); });

            // send empty to inform that is finished
            _eventLoop->postChannelEvent(std::function<void(void)>());
        }
        else
        {
            return _completeHandler.callAll();
        }
    }

    void ReadOnlyChannel::errorAll(const std::exception &error) const
    {
        if (_eventLoop)
        {
            _eventLoop->postChannelEvent([this, error]()
                                         { _errorHandler.callAll(error); });
        }
        else
        {
            return _errorHandler.callAll(error);
        }
    }

    void ReadOnlyChannel::setChannelEventLoop(IChannelEventLoop *eventLoop)
    {
        _eventLoop = eventLoop;
    }

    void ReadOnlyChannel::nextAllImpl(const std::string &msg) const
    {
        _messageHandler.callAll(msg);
        if (!_dataHandler.empty())
        {
            std::vector<char> data(msg.begin(), msg.end());
            _dataHandler.callAll(data);
        }
    }

    void ReadOnlyChannel::nextAllImpl(const std::vector<char> &data) const
    {
        _dataHandler.callAll(data);
        if (!_messageHandler.empty())
        {
            std::string msg(data.begin(), data.end());
            _messageHandler.callAll(msg);
        }
    }
}