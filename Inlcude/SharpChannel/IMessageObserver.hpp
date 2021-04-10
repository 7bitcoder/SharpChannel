#pragma once
#include <string>
#include "IObserver.hpp"

namespace cm
{

    struct IMessageObserver : public virtual IObserver
    {
        virtual void onMessageReceived(const std::string &message) = 0;

        virtual ~IMessageObserver() {}
    };
}