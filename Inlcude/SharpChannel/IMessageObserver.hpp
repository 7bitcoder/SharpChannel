#pragma once
#include <string>

namespace cm
{

    struct IMessageObserver 
    {
        virtual void next(const std::string &message) = 0;

        virtual ~IMessageObserver() {}
    };
}