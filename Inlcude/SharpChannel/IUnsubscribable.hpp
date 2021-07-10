#pragma once

namespace cm
{

    struct IUnsubscribable
    {
        using Ptr = std::shared_ptr<IUnsubscribable>;
        
        virtual bool unsubscribe() = 0;

        virtual ~IUnsubscribable() {}
    };
}