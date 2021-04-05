#include <iostream>
#include "IWriteOnlySubject.hpp"
#include "IReadOnlySubject.hpp"

namespace pm {
    struct ISubject: public virtual IReadOnlySubject, public virtual IWriteOnlySubject {
        
        virtual ~ISubject() {}
    };
}