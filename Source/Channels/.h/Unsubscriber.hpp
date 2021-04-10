#pragma once
#include <iostream>
#include <functional>
#include "IUnsubscribable.hpp"

namespace cm
{
    class Unsubscriber final : public IUnsubscribable
    {
    public:
        Unsubscriber(const std::function<bool()> &unsubscriber);
        bool unsunscribe();

        virtual ~Unsubscriber() {}
    private:
        const std::function<bool()> _unsubscriber;
    };
}