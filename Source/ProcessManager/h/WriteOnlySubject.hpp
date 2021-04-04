#include <iostream>
#include "ProcessComunicator.hpp"

namespace pm {
    struct WriteOnlySubject: public virtual ProcessComunicator {
        virtual ~WriteOnlySubject() {}
        virtual bool next(const std::string& msg) = 0;
    };
}