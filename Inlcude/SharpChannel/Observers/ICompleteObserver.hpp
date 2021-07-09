#pragma once

namespace cm
{

    struct ICompleteObserver 
    {
        virtual void complete() = 0;

        virtual ~ICompleteObserver() {}
    };
}