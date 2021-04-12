#include "Unsubscriber.hpp"


namespace cm {
    Unsubscriber::Unsubscriber(const std::function<bool()> &unsubscriber)
        : _unsubscriber(std::move(unsubscriber)) {}

    bool Unsubscriber::unsubscribe()
    {
        if (_unsubscriber)
        {
            return _unsubscriber();
        }
        return false;
    }
}