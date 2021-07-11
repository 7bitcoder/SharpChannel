#pragma once
#include <iostream>
#include <functional>
#include <mutex>
#include <memory>
#include <map>
#include "ICallbacks.hpp"

namespace cm
{
    template <class O, class... Args>
    class Callbacks final : public ICallbacks
    {
    public:
        using Ptr = std::shared_ptr<Callbacks<O, Args...>>;
        using Callback = std::function<O(Args...)>;

        CallbackId add(Callback &callback)
        {
            const std::lock_guard<std::mutex> lock(_guard);
            const CallbackId index = ++currentIndex;
            auto inserted = _callbacksMap.emplace(index, std::move(callback));
            return inserted.second ? index : 0;
        }

        bool remove(CallbackId callbackId) final
        {
            const std::lock_guard<std::mutex> lock(_guard);
            return _callbacksMap.erase(callbackId) == 1;
        }

        void callAll(Args... args) const
        {
            const std::lock_guard<std::mutex> lock(_guard);
            for (auto &pair : _callbacksMap)
            {
                pair.second(args...);
            }
        }

        void call(CallbackId id, Args... args) const
        {
            const std::lock_guard<std::mutex> lock(_guard);
            auto search = _callbacksMap.find(id);
            if (search != example.end())
            {
                search->second(args...);
            }
            else
            {
                // TODO
            }
        }

    private:
        CallbackId currentIndex = 0;
        mutable std::mutex _guard;
        std::map<size_t, const Callback> _callbacksMap;
    };
}