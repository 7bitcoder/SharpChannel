#pragma once
#include <iostream>
#include "IRunnable.hpp"

namespace cm {

    struct IWriteOnlyChannel: public virtual IRunnable {
        virtual bool sendMessage(const std::string& msg) = 0;
        virtual bool sendData(const char* data, size_t lenght) = 0;
        
        virtual ~IWriteOnlyChannel() {}
    };
}