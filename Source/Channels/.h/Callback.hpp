#pragma once
#include <iostream>
#include "IUnsubscribable.hpp"
#include "Control.hpp"
#include "IReadOnlyChannel.hpp"

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
        Control onMessageReceived(const std::string &msg);
        Control onDataReceived(const std::vector<char> &data);
        void onComplete();
        void onError(const std::exception& error); 

        // initialization
        Callback(const OnMessageReceived &onMessageReceived, const OnCompleted &onCompleted, const OnError &onError);

        Callback(const OnDataReceived &onDataReceived, const OnCompleted &onCompleted, const OnError &onError);

        ~Callback() {}
    private:
        OnMessageReceived _onMessageReceived;
        OnDataReceived _onDataReceived;
        OnError _onError;
        OnCompleted _onCompleted;
    };
}