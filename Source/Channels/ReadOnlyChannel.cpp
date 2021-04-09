#include <utility>
#include <functional>
#include "ReadOnlyChannel.hpp"
#include "IMessageObserver.hpp"
#include "IDataObserver.hpp"

namespace cm {

    std::shared_ptr<IUnsubscribable> ReadOnlyChannel::subscribe(const onCompleted& onCompleted) {
        return subscribeImpl(std::make_shared<Callback>(_callbacks, onCompleted));
    }

    std::shared_ptr<IUnsubscribable> ReadOnlyChannel::subscribe(const OnMessageReceived& onMessageReceived) {
        return subscribeImpl(std::make_shared<Callback>(_callbacks, onMessageReceived));
    }

    std::shared_ptr<IUnsubscribable> ReadOnlyChannel::subscribe(const OnMessageReceived& onMessageReceived, const onCompleted& onCompleted) {
        return subscribeImpl(std::make_shared<Callback>(_callbacks, onMessageReceived, onCompleted));
    }

    std::shared_ptr<IUnsubscribable> ReadOnlyChannel::subscribe(IMessageObserver& observer) {
        OnMessageReceived onMessageReceived = std::bind(&IMessageObserver::onMessageReceived, &observer, std::placeholders::_1);
        onCompleted onCompleted = std::bind(&IMessageObserver::onCompleted, &observer);
        return subscribeImpl(std::make_shared<Callback>(_callbacks, onMessageReceived, onCompleted));
    }

    std::shared_ptr<IUnsubscribable> ReadOnlyChannel::subscribe(const OnDataReceived& onMessageReceived) {
        return subscribeImpl(std::make_shared<Callback>(_callbacks, onMessageReceived));
    }

    std::shared_ptr<IUnsubscribable> ReadOnlyChannel::subscribe(const OnDataReceived& onDataReceived, const onCompleted& onCompleted) {
        return subscribeImpl(std::make_shared<Callback>(_callbacks, onDataReceived, onCompleted));
    }

    std::shared_ptr<IUnsubscribable> ReadOnlyChannel::subscribe(IDataObserver& observer) {
        OnDataReceived onDataReceived = std::bind(&IDataObserver::onDataReceived, &observer, std::placeholders::_1, std::placeholders::_2);
        onCompleted onCompleted = std::bind(&IDataObserver::onCompleted, &observer);
        return subscribeImpl(std::make_shared<Callback>(_callbacks, onDataReceived, onCompleted));
    }

    std::shared_ptr<IUnsubscribable> ReadOnlyChannel::subscribe(IObserver& observer) {
        onCompleted onCompleted = std::bind(&IObserver::onCompleted, &observer);
        return subscribeImpl(std::make_shared<Callback>(_callbacks, onCompleted));
    }

    void ReadOnlyChannel::nextAll(const std::string& msg) {
        if(_eventLoop) {
            _eventLoop->postChannelEvent([msg](){
                
            })
        } else {
            return nextAllImpl(msg);
        }
    } 

    void ReadOnlyChannel::nextAllImpl(const std::string& msg) {
        for(auto& pair: getCallbacks()) {
            auto& callback = pair.second;
            if (callback->isData()) {
                callback->onDataReceived(msg.c_str(), msg.size());
            } else if (callback->isMessage()) {
                callback->onMessageReceived(msg);
            }
        }
    }

    void ReadOnlyChannel::nextAll(const char* data, size_t lenght)   {
        std::unique_ptr<std::string> ptr;
        std::string f(data, lenght);
        for(auto& pair: getCallbacks()) {
            auto& callback = pair.second;
            if (callback->isData()) {
                callback->onDataReceived(data, lenght);
            } else if (callback->isMessage()) {
                if(!ptr) { // create if we need
                    ptr = std::make_unique<std::string>(data, lenght);
                }
                callback->onMessageReceived(*ptr);
            }
        }
    }   

    void ReadOnlyChannel::nextAllImpl(const char* data, size_t lenght)   {
        std::unique_ptr<std::string> ptr;
        std::string f(data, lenght);
        for(auto& pair: getCallbacks()) {
            auto& callback = pair.second;
            if (callback->isData()) {
                callback->onDataReceived(data, lenght);
            } else if (callback->isMessage()) {
                if(!ptr) { // create if we need
                    ptr = std::make_unique<std::string>(data, lenght);
                }
                callback->onMessageReceived(*ptr);
            }
        }
    }  

    void ReadOnlyChannel::completeAll() {
        for(auto& pair: getCallbacks()) {
            pair.second->complete();
        }
    } 

    void ReadOnlyChannel::completeAllImpl() {
        for(auto& pair: getCallbacks()) {
            pair.second->complete();
        }
    } 

    std::shared_ptr<IUnsubscribable> ReadOnlyChannel::subscribeImpl(std::shared_ptr<Callback> callback) {
        auto index = _indexCnt++;
        callback->setIdentifier(index);
        auto inserted = getCallbacks().insert(std::make_pair(index, callback));
        if(inserted.second) {
            return inserted.first->second;
        }
        return nullptr;
    }

    void ReadOnlyChannel::setChannelEventLoop(IChannelEventLoop& eventLoop) {
        _eventLoop = &eventLoop;
    }
}