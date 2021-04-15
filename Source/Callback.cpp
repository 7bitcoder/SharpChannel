#include <utility>
#include "ReadOnlyChannel.hpp"
#include "Callback.hpp"

namespace cm
{
    void Callback::onMessageReceived(const std::string &msg)
    {
        if (_onMessageReceived)
        {
            return _onMessageReceived(msg);
        };
    }

    void Callback::onDataReceived(const std::vector<char> &data)
    {
        if (_onDataReceived)
        {
            return _onDataReceived(data);
        }
    }

    void Callback::onComplete()
    {
        if (_onCompleted)
        {
            _onCompleted();
        }
    }

    void Callback::onConnected()
    {
        if (_onConnected)
        {
            _onConnected();
        }
    }

    void Callback::onError(const std::exception& error) {
        if(_onError) {
            _onError(error);
        }
    }

    Callback::Callback(const OnMessageReceived &onMessageReceived, const OnCompleted &onCompleted, const OnError &onError, const OnConnected &onConnected)
        : _onMessageReceived(onMessageReceived), _onCompleted(onCompleted), _onError(onError), _onConnected(onConnected) {}

    Callback::Callback(const OnDataReceived &onDataReceived, const OnCompleted &onCompleted, const OnError &onError, const OnConnected &onConnected)
        : _onDataReceived(onDataReceived), _onCompleted(onCompleted), _onError(onError), _onConnected(onConnected) {}
}