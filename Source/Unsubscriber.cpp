#include <vector>
#include "Unsubscriber.hpp"

namespace cm
{
    Unsubscriber::Unsubscriber(const std::function<bool()> &unsubscriber)
        : _unsubscriber(std::move(unsubscriber)) {}

    Unsubscriber::Unsubscriber(std::initializer_list<IUnsubscribable::Ptr> unsubscribers)
        : _unsubscriber([unsubs = std::vector<IUnsubscribable::Ptr>(unsubscribers.begin(), unsubscribers.end())]()
                        {
                            bool result = !unsubs.empty();
                            for (auto &unsub : unsubs)
                            {
                                if (unsub)
                                {
                                    bool unsubResult = unsub->unsubscribe(); // prevent optimalization
                                    result = unsubResult && result;
                                }
                            }
                            return result;
                        })
    {
    }

    Unsubscriber::Ptr Unsubscriber::Combine(std::initializer_list<IUnsubscribable::Ptr> unsubscribers)
    {
        return std::make_shared<Unsubscriber>(unsubscribers);
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