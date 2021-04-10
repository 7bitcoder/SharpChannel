#pragma once
#include <iostream>
#include <functional>
#include <mutex>
#include <memory>
#include <map>
#include "Callback.hpp"
#include "Unsubscriber.hpp"

namespace cm
{

    class CallbacksMap
    {
    public:
        std::unique_ptr<Unsubscriber> insert(const OnDataReceived &onDataReceived, const OnCompleted &onCompleted = OnCompleted(), const OnError &onError = OnError());
        std::unique_ptr<Unsubscriber> insert(const OnMessageReceived &onMessageReceived, const OnCompleted &onCompleted = OnCompleted(), const OnError &onError = OnError());

        bool remove(const size_t index);
        void completeAll();
        void nextAll(const std::string &msg);
        void nextAll(const std::vector<char> &data);
        void errorAll(const std::exception& error);

    private:
        std::unique_ptr<Unsubscriber> insertCallback(std::shared_ptr<Callback> callback);
        size_t currentIndex = 0;
        std::mutex _guard;
        std::map<size_t, std::shared_ptr<Callback>> _map;
    };
}