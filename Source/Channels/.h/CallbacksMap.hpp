#pragma once
#include <iostream>
#include <functional>
#include <mutex>
#include <memory>
#include <map>
#include "Callback.hpp"

namespace cm
{

    class CallbacksMap
    {
    public:
        std::shared_ptr<Callback> insert(const OnDataReceived &onDataReceived, const onCompleted &onCompleted = onCompleted());
        std::shared_ptr<Callback> insert(const OnMessageReceived &onMessageReceived, const onCompleted &onCompleted = onCompleted());

        bool remove(const size_t index);
        void completeAll();
        void nextAll(const std::string &msg);
        void nextAll(const std::vector<char> &data);

    private:
        std::shared_ptr<Callback> insertImpl(std::shared_ptr<Callback> callback, size_t index);
        size_t currentIndex = 0;
        std::mutex _guard;
        std::map<size_t, std::shared_ptr<Callback>> _map;
    };
}