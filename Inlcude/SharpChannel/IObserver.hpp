#pragma once

namespace cm
{

    struct IObserver
    {
        virtual void onCompleted() = 0;

        virtual ~IObserver() {}
    };
}