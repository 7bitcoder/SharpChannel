#pragma once
#include <iostream>
#include "Control.hpp"

namespace cm {
    
    struct IObserver {
        virtual void onComplete() = 0;

        virtual ~IObserver() {}
    };
}