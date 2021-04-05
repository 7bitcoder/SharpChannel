#pragma once
#include <iostream>
#include "IProcessComunicator.hpp"

namespace pm {

    struct IWriteOnlySubject: public virtual IProcessComunicator {
        virtual bool next(const std::string& msg) = 0;
        
        virtual ~IWriteOnlySubject() {}
    };
}