#pragma once

namespace cm
{
    struct ICallbacks
    {
        using CallbackId = size_t;

        virtual bool remove(CallbackId callbackId) = 0;
        virtual bool empty() const = 0;

        virtual ~ICallbacks() {}
    };
}