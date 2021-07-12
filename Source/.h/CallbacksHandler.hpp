#pragma once
#include <iostream>
#include <functional>
#include <mutex>
#include <memory>
#include <map>
#include "Unsubscriber.hpp"
#include "Callbacks.hpp"

namespace cm
{
    template <class O, class... Args>
    class CallbacksHandler final : public ICallbacks
    {
    public:
        using Ptr = std::shared_ptr<CallbacksHandler<O, Args...>>;
        using CallbacksPtr = std::shared_ptr<Callbacks<O, Args...>>;
        using Callback = std::function<O(Args...)>;

        Unsubscriber::Ptr add(const Callback &callback)
        {
            if (!callback)
            {
                return nullptr;
            }
            auto callBackId = _callbacks->add(callback);
            if (callBackId)
            {
                auto unsubscriber = [weakHandler = std::weak_ptr<Callbacks<O, Args...>>(_callbacks), callBackId]()
                {
                    auto weakHandlerShared = weakHandler.lock();
                    if (weakHandlerShared)
                    {
                        return weakHandlerShared->remove(callBackId);
                    }
                    else
                    {
                        // TODO throw exception;
                        return false;
                    }
                };
                return std::make_unique<Unsubscriber>(unsubscriber);
            }
            return nullptr;
        }

        bool remove(CallbackId callbackId) final
        {
            return _callbacks->remove(callbackId);
        }

        void callAll(Args... args) const
        {
            return _callbacks->callAll(args...);
        }

        void call(CallbackId callbackId, Args... args) const
        {
            return _callbacks->call(callbackId, args...);
        }

        bool empty() const
        {
            return _callbacks->empty();
        }

    private:
        CallbacksPtr _callbacks = std::make_shared<Callbacks<O, Args...>>();
    };
}