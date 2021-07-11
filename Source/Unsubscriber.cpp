#include "Unsubscriber.hpp"

namespace cm
{
    Unsubscriber::Unsubscriber(const std::function<bool()> &unsubscriber)
        : _unsubscriber(std::move(unsubscriber)) {}

    Unsubscriber::Unsubscriber(std::initializer_list<IUnsubscribable::Ptr> unsubscribers)
        : _unsubscriber([&unsubscribers]()
                        {
                            bool result = true;
                            for (auto &unsub : unsubscribers)
                            {
                                result = unsub->unsubscribe() && result;
                            }
                            return result;
                        })
    {
    }

    bool Unsubscriber::unsubscribe()
    {
        if (_unsubscriber)
        {
            return _unsubscriber();
        }
        return false;
    }
}