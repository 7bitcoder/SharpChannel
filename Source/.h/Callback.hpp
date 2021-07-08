#pragma once
#include <iostream>
#include "IUnsubscribable.hpp"
#include "IChannelObservable.hpp"

namespace cm
{
    class Callback
    {
    public:
        // getInfo
        bool isData() { return bool(_onDataReceived); }
        bool isMessage() { return bool(_onMessageReceived); }
        bool isComplete() { return bool(_onCompleted); }
        // callable
        void onMessageReceived(const std::string &msg);
        void onDataReceived(const std::vector<char> &data);
        void onComplete();
        void onConnected();
        void onError(const std::exception& error); 

        // initialization
        Callback(const OnMessageReceived &onMessageReceived, const OnCompleted &onCompleted, const OnError &onError, const OnConnected &onConnected);

        Callback(const OnDataReceived &onDataReceived, const OnCompleted &onCompleted, const OnError &onError, const OnConnected &onConnected);

        ~Callback() {}
    private:
        OnMessageReceived _onMessageReceived;
        OnDataReceived _onDataReceived;
        OnError _onError;
        OnCompleted _onCompleted;
        OnConnected _onConnected;
    };
}