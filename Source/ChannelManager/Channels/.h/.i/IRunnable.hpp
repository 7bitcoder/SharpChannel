#pragma once
#include <iostream>
#include <functional>
#include "Control.hpp"

namespace cm {

    struct IRunnable {
        virtual void run() = 0;
        virtual void finish() {};
        
        virtual ~IRunnable() {}
    };
}