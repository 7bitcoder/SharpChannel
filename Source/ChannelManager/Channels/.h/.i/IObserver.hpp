#pragma once
#include <iostream>
#include "Control.hpp"

namespace cm {
    
    struct IObserver {
        virtual void onCompleted() = 0;

        virtual ~IObserver() {}
    };
}