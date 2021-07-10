#pragma once
#include <iostream>
#include <functional>
#include "IUnsubscribable.hpp"

namespace cm
{
    class Unsubscriber final : public IUnsubscribable
    {
    public:
        using Ptr = std::shared_ptr<Unsubscriber>;

        Unsubscriber(const std::function<bool()> &unsubscriber);
        
        bool unsubscribe();
    private:
        const std::function<bool()> _unsubscriber;
    };
}