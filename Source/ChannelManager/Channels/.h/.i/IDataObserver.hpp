#pragma once
#include <iostream>
#include "Control.hpp"
#include "IObserver.hpp"

namespace cm {
    
    struct IDataObserver: public virtual IObserver {
        virtual Control onDataReceived(const char* data, size_t lenght) = 0;

        virtual ~IDataObserver() {}
    };
}