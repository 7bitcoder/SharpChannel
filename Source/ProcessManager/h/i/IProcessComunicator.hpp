#pragma once
#include <iostream>
#include <functional>
#include "Control.hpp"

namespace pm {

    struct IProcessComunicator {
        virtual void run() = 0;
        virtual void finish() {};
        
        virtual ~IProcessComunicator() {}
    };
}