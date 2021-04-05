#include <iostream>
#include "ISubject.hpp"
#include "ReadOnlySubject.hpp"
#include "WriteOnlySubject.hpp"

namespace pm {
    struct Subject: public ISubject, public ReadOnlySubject, public WriteOnlySubject {
        
        virtual ~Subject() {}
    };
}