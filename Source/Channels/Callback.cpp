#include <utility>
#include "ReadOnlyChannel.hpp"
#include "Callback.hpp"

namespace cm
{
    Control Callback::onMessageReceived(const std::string &msg)
    {
        if (_onMessageReceived)
        {
            return _onMessageReceived(msg);
        }
        return Control::Stop;
    }

    Control Callback::onDataReceived(const std::vector<char> &data)
    {
        if (_onDataReceived)
        {
            return _onDataReceived(data);
        }
        return Control::Stop;
    }

    void Callback::onComplete()
    {
        if (_onCompleted)
        {
            _onCompleted();
        }
    }

    void Callback::onError(const std::exception& error) {
        if(_onError) {
            onError(error);
        }
    }

    Callback::Callback(const OnMessageReceived &onMessageReceived, const OnCompleted &onCompleted, const OnError &onError)
        : _onMessageReceived(std::move(onMessageReceived)), _onCompleted(std::move(onCompleted)), _onError(std::move(onError)) {}

    Callback::Callback(const OnDataReceived &onDataReceived, const OnCompleted &onCompleted, const OnError &onError)
        : _onDataReceived(std::move(onDataReceived)), _onCompleted(std::move(onCompleted)), _onError(std::move(onError)) {}
}