#include "CallbacksMap.hpp"

namespace cm
{
    std::unique_ptr<Unsubscriber> CallbacksMap::insert(const OnDataReceived &onDataReceived, const OnCompleted &onCompleted, const OnError &onError, const OnConnected &onConnected)
    {
        const std::lock_guard<std::mutex> lock(_guard);
        auto callback = std::make_shared<Callback>(onDataReceived, onCompleted, onError, onConnected);
        return insertCallback(callback);
    }

    std::unique_ptr<Unsubscriber> CallbacksMap::insert(const OnMessageReceived &onMessageReceived, const OnCompleted &onCompleted, const OnError &onError, const OnConnected &onConnected)
    {
        const std::lock_guard<std::mutex> lock(_guard);
        auto callback = std::make_shared<Callback>(onMessageReceived, onCompleted, onError, onConnected);
        return insertCallback(callback);
    }

    std::unique_ptr<Unsubscriber> CallbacksMap::insertCallback(std::shared_ptr<Callback> callback)
    {
        auto index = currentIndex++;
        auto inserted = _map.insert(std::make_pair(index, callback));
        if (inserted.second)
        {
            auto unsubscriber = [weakThis = weak_from_this(), index]()
            {
                auto weakThisShared = weakThis.lock();
                if (weakThisShared)
                {
                    return weakThisShared->remove(index);
                }
                else
                {
                    // TODO throw exception;
                }
            };
            return std::make_unique<Unsubscriber>(unsubscriber);
        }
        else
        {
            return nullptr;
        }
    }

    bool CallbacksMap::remove(const size_t index)
    {
        const std::lock_guard<std::mutex> lock(_guard);
        return _map.erase(index) == 1;
    }

    void CallbacksMap::completeAll()
    {
        const std::lock_guard<std::mutex> lock(_guard);
        for (auto &pair : _map)
        {
            pair.second->onComplete();
        }
    }

    void CallbacksMap::connectedAll()
    {
        const std::lock_guard<std::mutex> lock(_guard);
        for (auto &pair : _map)
        {
            pair.second->onConnected();
        }
    }

    void CallbacksMap::nextAll(const std::string &msg)
    {
        const std::lock_guard<std::mutex> lock(_guard);
        std::unique_ptr<std::vector<char>> ptr;
        for (auto &pair : _map)
        {
            auto &callback = pair.second;
            if (callback->isData())
            {
                if (!ptr)
                { // create if we need
                    ptr = std::make_unique<std::vector<char>>(msg.begin(), msg.end());
                }
                callback->onDataReceived(*ptr);
            }
            else if (callback->isMessage())
            {
                callback->onMessageReceived(msg);
            }
        }
    }

    void CallbacksMap::nextAll(const std::vector<char> &data)
    {
        const std::lock_guard<std::mutex> lock(_guard);
        std::unique_ptr<std::string> ptr;
        for (auto &pair : _map)
        {
            auto &callback = pair.second;
            if (callback->isData())
            {
                callback->onDataReceived(data);
            }
            else if (callback->isMessage())
            {
                if (!ptr)
                { // create if we need
                    ptr = std::make_unique<std::string>(data.begin(), data.end());
                }
                callback->onMessageReceived(*ptr);
            }
        }
    }

    void CallbacksMap::errorAll(const std::exception &error)
    {
        const std::lock_guard<std::mutex> lock(_guard);
        for (auto &pair : _map)
        {
            pair.second->onError(error);
        }
    }
}