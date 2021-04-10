#include <utility>
#include <functional>
#include "ReadOnlyChannel.hpp"
#include "IMessageObserver.hpp"
#include "IDataObserver.hpp"

namespace cm {

    std::shared_ptr<IUnsubscribable> ReadOnlyChannel::subscribe(const onCompleted& onCompleted) {
        return _callbacksMap.insert(OnMessageReceived(), onCompleted);
    }

    std::shared_ptr<IUnsubscribable> ReadOnlyChannel::subscribe(const OnMessageReceived& onMessageReceived) {
        return _callbacksMap.insert(onMessageReceived);
    }

    std::shared_ptr<IUnsubscribable> ReadOnlyChannel::subscribe(const OnMessageReceived& onMessageReceived, const onCompleted& onCompleted) {
        return _callbacksMap.insert(onMessageReceived, onCompleted);
    }

    std::shared_ptr<IUnsubscribable> ReadOnlyChannel::subscribe(IMessageObserver& observer) {
        OnMessageReceived onMessageReceived = std::bind(&IMessageObserver::onMessageReceived, &observer, std::placeholders::_1);
        onCompleted onCompleted = std::bind(&IMessageObserver::onCompleted, &observer);
        return _callbacksMap.insert(onMessageReceived, onCompleted);
    }

    std::shared_ptr<IUnsubscribable> ReadOnlyChannel::subscribe(const OnDataReceived& onDataReceived) {
        return _callbacksMap.insert(onDataReceived);
    }

    std::shared_ptr<IUnsubscribable> ReadOnlyChannel::subscribe(const OnDataReceived& onDataReceived, const onCompleted& onCompleted) {
        return _callbacksMap.insert(onDataReceived, onCompleted);
    }

    std::shared_ptr<IUnsubscribable> ReadOnlyChannel::subscribe(IDataObserver& observer) {
        OnDataReceived onDataReceived = std::bind(&IDataObserver::onDataReceived, &observer, std::placeholders::_1);
        onCompleted onCompleted = std::bind(&IDataObserver::onCompleted, &observer);
        return _callbacksMap.insert(onDataReceived, onCompleted);
    }

    std::shared_ptr<IUnsubscribable> ReadOnlyChannel::subscribe(IObserver& observer) {
        onCompleted onCompleted = std::bind(&IObserver::onCompleted, &observer);
        return _callbacksMap.insert(OnDataReceived(), onCompleted);
    }

    void ReadOnlyChannel::nextAll(const std::string& msg) {
        if(_eventLoop) {
            _eventLoop->postChannelEvent([this, msg](){
                _callbacksMap.nextAll(msg);
            });
        } else {
            return _callbacksMap.nextAll(msg);
        }
    } 

    void ReadOnlyChannel::nextAll(const std::vector<char>& data)   {
        if(_eventLoop) {
            _eventLoop->postChannelEvent([this, data](){
                _callbacksMap.nextAll(data);
            });
        } else {
            return _callbacksMap.nextAll(data);
        }
    }   

    void ReadOnlyChannel::completeAll() {
        if(_eventLoop) {
            _eventLoop->postChannelEvent([this](){
                _callbacksMap.completeAll();
            });
        } else {
            return _callbacksMap.completeAll();
        }
    } 

    void ReadOnlyChannel::setChannelEventLoop(IChannelEventLoop* eventLoop) {
        _eventLoop = eventLoop;
    }
}