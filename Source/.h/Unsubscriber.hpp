#pragma once
#include <iostream>
#include <functional>
#include <initializer_list>
#include "IUnsubscribable.hpp"

namespace cm
{
    class Unsubscriber final : public IUnsubscribable
    {
    public:
        using Ptr = std::shared_ptr<Unsubscriber>;

        Unsubscriber(const std::function<bool()> &unsubscriber);
        Unsubscriber(std::initializer_list<IUnsubscribable::Ptr> unsubscribers);
        
        bool unsubscribe();
    private:
        const std::function<bool()> _unsubscriber;
    };
}