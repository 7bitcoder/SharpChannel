#pragma once
#include <iostream>
#include <functional>
#include <mutex>
#include <memory>
#include <map>
#include "Channels/Callback.hpp"
#include "Unsubscriber.hpp"

namespace cm
{

    class CallbacksHandler: public std::enable_shared_from_this<CallbacksHandler>
    {
    public:
        using Ptr = std::shared_ptr<CallbacksHandler>; 
        using CallbackId = size_t;

        CallbackId addCallback(Callback& callback);
        bool removeCallback(CallbackId callbackId);

        void completeAll() const;
        void nextAll(const std::string &msg) const;
        void nextAll(const std::vector<char> &data) const;
        void errorAll(const std::exception& error) const;

    private:
        size_t currentIndex = 0;
        mutable std::mutex _guard;
        std::map<size_t, const Callback> _map;
    };
}