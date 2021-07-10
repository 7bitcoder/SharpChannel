#include <utility>
#include "Channels/Callback.hpp"

namespace cm
{
    void Callback::onMessageReceived(const std::string &msg) const
    {
        if (_onMessageReceived)
        {
            return _onMessageReceived(msg);
        };
    }

    void Callback::onDataReceived(const std::vector<char> &data) const
    {
        if (_onDataReceived)
        {
            return _onDataReceived(data);
        }
    }

    void Callback::onComplete() const
    {
        if (_onCompleted)
        {
            _onCompleted();
        }
    }

    void Callback::onError(const std::exception &error) const
    {
        if (_onError)
        {
            _onError(error);
        }
    }

    Callback::Callback(const OnMessageReceived &onMessageReceived, const OnCompleted &onCompleted, const OnError &onError)
        : _onMessageReceived(onMessageReceived), _onCompleted(onCompleted), _onError(onError) {}

    Callback::Callback(const OnDataReceived &onDataReceived, const OnCompleted &onCompleted, const OnError &onError)
        : _onDataReceived(onDataReceived), _onCompleted(onCompleted), _onError(onError) {}
}