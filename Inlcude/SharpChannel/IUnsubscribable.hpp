#pragma once

namespace cm
{

    struct IUnsubscribable
    {
        virtual bool unsubscribe() = 0;

        virtual ~IUnsubscribable() {}
    };
}