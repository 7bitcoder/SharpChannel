#pragma once
#include <iostream>
#include <functional>
#include <map>
#include "IWriteOnlySubject.hpp"

namespace pm {
    class WriteOnlySubject: public virtual IWriteOnlySubject {
        public:

            virtual ~WriteOnlySubject() {}
    };
}