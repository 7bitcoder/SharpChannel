#pragma once
#include <iostream>

namespace pm {
    
    struct IUnsubscribable {
        virtual bool unsunscribe() = 0;

        virtual ~IUnsubscribable() {}
    };
}