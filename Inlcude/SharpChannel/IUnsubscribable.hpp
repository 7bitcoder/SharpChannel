#pragma once

namespace cm
{

    struct IUnsubscribable
    {
        virtual bool unsunscribe() = 0;

        virtual ~IUnsubscribable() {}
    };
}