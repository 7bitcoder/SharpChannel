#include <utility>
#include <functional>
#include "ReadOnlyChannel.hpp"

namespace cm
{

    IUnsubscribable::Ptr ReadOnlyChannel::subscribe(const OnMessageReceived &onMessageReceived)
    {
        return _messageCallbacks.add(onMessageReceived);
    }

    IUnsubscribable::Ptr ReadOnlyChannel::subscribe(const OnDataReceived &onDataReceived)
    {
        return _dataCallbacks.add(onDataReceived);
    }

    IUnsubscribable::Ptr ReadOnlyChannel::subscribe(const OnCompleted &onCompleted)
    {
        return _completeCallbacks.add(onCompleted);
    }

    IUnsubscribable::Ptr ReadOnlyChannel::subscribe(const OnError &onError)
    {
        return _errorCallbacks.add(onError);
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
                                         { _completeCallbacks.callAll(); });

            // send empty to inform that is finished
            _eventLoop->postChannelEvent(std::function<void(void)>());
        }
        else
        {
            return _completeCallbacks.callAll();
        }
    }

    void ReadOnlyChannel::errorAll(const std::exception &error) const
    {
        if (_eventLoop)
        {
            _eventLoop->postChannelEvent([this, error]()
                                         { _errorCallbacks.callAll(error); });
        }
        else
        {
            return _errorCallbacks.callAll(error);
        }
    }

    void ReadOnlyChannel::setChannelEventLoop(IChannelEventLoop *eventLoop)
    {
        _eventLoop = eventLoop;
    }

    void ReadOnlyChannel::nextAllImpl(const std::string &msg) const
    {
        _messageCallbacks.callAll(msg);
        if (!_dataCallbacks.empty())
        {
            std::vector<char> data(msg.begin(), msg.end());
            _dataCallbacks.callAll(data);
        }
    }

    void ReadOnlyChannel::nextAllImpl(const std::vector<char> &data) const
    {
        _dataCallbacks.callAll(data);
        if (!_messageCallbacks.empty())
        {
            std::string msg(data.begin(), data.end());
            _messageCallbacks.callAll(msg);
        }
    }
}