#pragma once
#include <iostream>
#include "IRunnable.hpp"

namespace cm {

    struct IWriteOnlyChannel: public virtual IRunnable {
        virtual bool send(const std::string& msg) = 0;
        virtual bool send(const char* data, size_t lenght) = 0;
        
        virtual ~IWriteOnlyChannel() {}
    };
}