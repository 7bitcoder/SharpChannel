#include <utility>
#include <functional>
#include "ReadOnlyChannel.hpp"
#include "IMessageObserver.hpp"
#include "IDataObserver.hpp"

namespace cm {

    std::shared_ptr<IUnsubscribable> ReadOnlyChannel::subscribe(const OnComplete& onComplete) {
        return subscribeImpl(std::make_shared<Callback>(_callbacks, onComplete));
    }

    std::shared_ptr<IUnsubscribable> ReadOnlyChannel::subscribe(const OnMessageReceived& onMessageReceived) {
        return subscribeImpl(std::make_shared<Callback>(_callbacks, onMessageReceived));
    }

    std::shared_ptr<IUnsubscribable> ReadOnlyChannel::subscribe(const OnMessageReceived& onMessageReceived, const OnComplete& onComplete) {
        return subscribeImpl(std::make_shared<Callback>(_callbacks, onMessageReceived, onComplete));
    }

    std::shared_ptr<IUnsubscribable> ReadOnlyChannel::subscribe(IMessageObserver& observer) {
        OnMessageReceived onMessageReceived = std::bind(&IMessageObserver::onMessageReceived, &observer, std::placeholders::_1);
        OnComplete onComplete = std::bind(&IMessageObserver::onComplete, &observer);
        return subscribeImpl(std::make_shared<Callback>(_callbacks, onMessageReceived, onComplete));
    }

    std::shared_ptr<IUnsubscribable> ReadOnlyChannel::subscribe(const OnDataReceived& onMessageReceived) {
        return subscribeImpl(std::make_shared<Callback>(_callbacks, onMessageReceived));
    }

    std::shared_ptr<IUnsubscribable> ReadOnlyChannel::subscribe(const OnDataReceived& onRawReceive, const OnComplete& onComplete) {
        return subscribeImpl(std::make_shared<Callback>(_callbacks, onRawReceive, onComplete));
    }

    std::shared_ptr<IUnsubscribable> ReadOnlyChannel::subscribe(IDataObserver& observer) {
        OnDataReceived onRawReceive = std::bind(&IDataObserver::onDataReceived, &observer, std::placeholders::_1, std::placeholders::_2);
        OnComplete onComplete = std::bind(&IDataObserver::onComplete, &observer);
        return subscribeImpl(std::make_shared<Callback>(_callbacks, onRawReceive, onComplete));
    }

    void ReadOnlyChannel::nextAll(const std::string& msg) {
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

    void ReadOnlyChannel::completeAll() {
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

}