#include <utility>
#include "ReadOnlyChannel.hpp"
#include "Callback.hpp"

namespace cm {
    bool Callback::unsunscribe() {
        if(_unsubscriber) {
            return _unsubscriber();
        }
        return false;
    }

    Control Callback::onMessageReceived(const std::string& msg) {
        if(_onMessageReceived) {
            return _onMessageReceived(msg);
        }
        return Control::Stop;
    }

    Control Callback::onDataReceived(const std::vector<char>& data) {
        if(_onDataReceived) {
            return _onDataReceived(data);
        }
        return Control::Stop;
    }

    void Callback::complete() {
        if(_onCompleted) {
            _onCompleted();
        }
    }

    Callback::Callback(const std::function<bool()>& unsubscriber, const OnMessageReceived& onMessageReceived, const onCompleted& onCompleted)
        : _unsubscriber(unsubscriber), _onMessageReceived(std::move(onMessageReceived)), _onCompleted(std::move(onCompleted)) {}

    Callback::Callback(const std::function<bool()>& unsubscriber, const OnDataReceived& onDataReceived, const onCompleted& onCompleted)
        : _unsubscriber(unsubscriber), _onDataReceived(std::move(onDataReceived)), _onCompleted(std::move(onCompleted)) {}
}