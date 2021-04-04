#include <iostream>
#include "WriteOnlySubject.hpp"
#include "ReadOnlySubject.hpp"

namespace pm {
    struct Subject: public ReadOnlySubject, public WriteOnlySubject {
        virtual ~Subject() {}
    };
}