#pragma once
#include <iostream>

namespace pm {
    
    struct IObserver {
        virtual Control next(const std::string& message) = 0;
        virtual void complete() = 0;

        virtual ~IObserver() {}
    };
}