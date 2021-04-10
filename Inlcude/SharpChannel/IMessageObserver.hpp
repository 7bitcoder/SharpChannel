#pragma once
#include <string>
#include "Control.hpp"
#include "IObserver.hpp"

namespace cm
{

    struct IMessageObserver : public virtual IObserver
    {
        virtual Control onMessageReceived(const std::string &message) = 0;

        virtual ~IMessageObserver() {}
    };
}