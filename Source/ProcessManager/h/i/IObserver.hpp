#pragma once
#include <iostream>

namespace pm {
    
    struct IObserver {
        virtual Control onReceive(const std::string& message) = 0;
        virtual void onComplete() = 0;

        virtual ~IObserver() {}
    };
}