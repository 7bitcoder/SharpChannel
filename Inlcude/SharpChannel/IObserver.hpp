#pragma once

namespace cm
{

    struct IObserver
    {
        virtual void onCompleted() = 0;
        virtual void onConnected() = 0;
        virtual void onError(const std::exception& e) = 0;

        virtual ~IObserver() {}
    };
}