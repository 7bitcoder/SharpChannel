#pragma once
#include <iostream>
#include "IUnsubscribable.hpp"
#include "IObservables.hpp"

namespace cm
{
    class Callback
    {
    public:
        // getInfo
        bool isData() const { return bool(_onDataReceived); }
        bool isMessage() const { return bool(_onMessageReceived); }
        bool isComplete() const { return bool(_onCompleted); }
        // callable
        void onMessageReceived(const std::string &msg) const;
        void onDataReceived(const std::vector<char> &data) const;
        void onComplete() const;
        void onError(const std::exception& error) const; 

        // initialization
        Callback(const OnMessageReceived &onMessageReceived, const OnCompleted &onCompleted = OnCompleted(), const OnError &onError = OnError());

        Callback(const OnDataReceived &onDataReceived, const OnCompleted &onCompleted = OnCompleted(), const OnError &onError = OnError());

        Callback( Callback && ) = default;
    private:
        const OnMessageReceived _onMessageReceived;
        const OnDataReceived _onDataReceived;
        const OnError _onError;
        const OnCompleted _onCompleted;
    };
}