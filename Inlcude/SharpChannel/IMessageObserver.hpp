#pragma once
#include <string>

namespace cm
{

    struct IMessageObserver 
    {
        virtual bool next(const std::string &message) = 0;

        virtual ~IMessageObserver() {}
    };
}