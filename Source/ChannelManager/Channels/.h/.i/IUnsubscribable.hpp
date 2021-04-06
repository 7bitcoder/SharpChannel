#pragma once
#include <iostream>

namespace cm {
    
    struct IUnsubscribable {
        virtual bool unsunscribe() = 0;

        virtual ~IUnsubscribable() {}
    };
}