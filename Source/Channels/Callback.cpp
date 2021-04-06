#include <utility>
#include "ReadOnlyChannel.hpp"
#include "Callback.hpp"

namespace cm {
    bool Callback::unsunscribe() {
        if(_map) {
            return _map->erase(_identifier) == 1;
        }
        return false;
    }

    Control Callback::onMessageReceived(const std::string& msg) {
        if(_onMessageReceived) {
            return _onMessageReceived(msg);
        }
        return Control::Stop;
    }

    Control Callback::onDataReceived(const char* data, size_t lenght) {
        if(_onDataReceived) {
            return _onDataReceived(data, lenght);
        }
        return Control::Stop;
    }

    void Callback::complete() {
        if(_onCompleted) {
            _onCompleted();
        }
    }

    Callback::Callback(std::shared_ptr<CallbacksMap> map, const onCompleted& onCompleted)
        : _map(map), _onCompleted(std::move(onCompleted)) {}

    Callback::Callback(std::shared_ptr<CallbacksMap> map, const OnMessageReceived& onMessageReceived)
        : _map(map), _onMessageReceived(std::move(onMessageReceived)) {}

    Callback::Callback(std::shared_ptr<CallbacksMap> map, const OnMessageReceived& onMessageReceived, const onCompleted& onCompleted)
        : _map(map), _onMessageReceived(std::move(onMessageReceived)), _onCompleted(std::move(onCompleted)) {}

    Callback::Callback(std::shared_ptr<CallbacksMap> map, const OnDataReceived& onDataReceived)
        : _map(map), _onDataReceived(std::move(onDataReceived)) {}

    Callback::Callback(std::shared_ptr<CallbacksMap> map, const OnDataReceived& onDataReceived, const onCompleted& onCompleted)
        : _map(map), _onDataReceived(std::move(onDataReceived)), _onCompleted(std::move(onCompleted)) {}
}