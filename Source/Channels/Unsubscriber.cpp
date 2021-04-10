#include "Unsubscriber.hpp"


namespace cm {
    Unsubscriber::Unsubscriber(const std::function<bool()> &unsubscriber)
        : _unsubscriber(std::move(unsubscriber)) {}

    bool Unsubscriber::unsunscribe()
    {
        if (_unsubscriber)
        {
            return _unsubscriber();
        }
        return false;
    }
}