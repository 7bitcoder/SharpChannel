#pragma once
#include <iostream>
#include <functional>
#include "Control.hpp"

namespace pm {

    struct ProcessComunicator {
        virtual ~ProcessComunicator() {}
        virtual void run() = 0;
    };
}