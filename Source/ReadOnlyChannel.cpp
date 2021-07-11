#include <utility>
#include <functional>
#include "ReadOnlyChannel.hpp"

namespace cm
{

    IUnsubscribable::Ptr ReadOnlyChannel::subscribe(const OnMessageReceived &onMessageReceived)
    {
        return makeCallback(onMessageReceived);
    }

    IUnsubscribable::Ptr ReadOnlyChannel::subscribe(const OnDataReceived &onDataReceived)
    {
        return makeCallback(onDataReceived);
    }

    IUnsubscribable::Ptr ReadOnlyChannel::subscribe(const OnCompleted &onCompleted)
    {
        return makeCallback(OnMessageReceived(), onCompleted);
    }

    IUnsubscribable::Ptr ReadOnlyChannel::subscribe(const OnError &onError)
    {
        return makeCallback(OnMessageReceived(), OnCompleted(), onError);
    }

    IUnsubscribable::Ptr ReadOnlyChannel::subscribe(const OnCompleted &onCompleted, const OnError &onError)
    {
        return makeCallback(OnMessageReceived(), onCompleted, onError);
    }

    IUnsubscribable::Ptr ReadOnlyChannel::subscribe(const OnMessageReceived &onMessageReceived, const OnCompleted &onCompleted, const OnError &onError)
    {
        return makeCallback(onMessageReceived, onCompleted, onError);
    }

    IUnsubscribable::Ptr ReadOnlyChannel::subscribe(const OnDataReceived &onDataReceived, const OnCompleted &onCompleted, const OnError &onError)
    {
        return makeCallback(onDataReceived, onCompleted, onError);
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

    IUnsubscribable::Ptr ReadOnlyChannel::makeCallback(const OnMessageReceived &onMessageReceived, const OnCompleted &onCompleted, const OnError &onError)
    {
        return registerCallback(Callback(onMessageReceived, onCompleted, onError));
    }

    IUnsubscribable::Ptr ReadOnlyChannel::makeCallback(const OnDataReceived &onDataReceived, const OnCompleted &onCompleted, const OnError &onError)
    {
        return registerCallback(Callback(onDataReceived, onCompleted, onError));
    }

    IUnsubscribable::Ptr ReadOnlyChannel::registerCallback(Callback &callback)
    {
        auto callBackId = _callbacksHandler->addCallback(callback);
        if (callBackId)
        {
            auto unsubscriber = [weakHandler = std::weak_ptr<CallbacksHandler>(_callbacksHandler), callBackId]()
            {
                auto weakHandlerShared = weakHandler.lock();
                if (weakHandlerShared)
                {
                    return weakHandlerShared->removeCallback(callBackId);
                }
                else
                {
                    // TODO throw exception;
                    return false;
                }
            };
            return std::make_unique<Unsubscriber>(unsubscriber);
        }
        return nullptr;
    }

    void ReadOnlyChannel::nextAll(const std::string &msg) const
    {
        if (_eventLoop)
        {
            _eventLoop->postChannelEvent([this, msg]()
                                         { _callbacksHandler->nextAll(msg); });
        }
        else
        {
            return _callbacksHandler->nextAll(msg);
        }
    }

    void ReadOnlyChannel::nextAll(const std::vector<char> &data) const
    {
        if (_eventLoop)
        {
            _eventLoop->postChannelEvent([this, data]()
                                         { _callbacksHandler->nextAll(data); });
        }
        else
        {
            return _callbacksHandler->nextAll(data);
        }
    }

    void ReadOnlyChannel::completeAll() const
    {
        if (_eventLoop)
        {
            _eventLoop->postChannelEvent([this]()
                                         { _callbacksHandler->completeAll(); });

            // send empty to inform that is finished
            _eventLoop->postChannelEvent(std::function<void(void)>());
        }
        else
        {
            return _callbacksHandler->completeAll();
        }
    }

    void ReadOnlyChannel::errorAll(const std::exception &error) const
    {
        if (_eventLoop)
        {
            _eventLoop->postChannelEvent([this, error]()
                                         { _callbacksHandler->errorAll(error); });
        }
        else
        {
            return _callbacksHandler->errorAll(error);
        }
    }

    void ReadOnlyChannel::setChannelEventLoop(IChannelEventLoop *eventLoop)
    {
        _eventLoop = eventLoop;
    }
}