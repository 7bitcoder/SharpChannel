#include "CallbacksMap.hpp"

namespace cm
{
    std::shared_ptr<Callback> CallbacksMap::insert(const OnDataReceived &onDataReceived, const onCompleted &onCompleted)
    {
        const std::lock_guard<std::mutex> lock(_guard);
        auto index = currentIndex++;
        auto unsubscriber = [this, index]() {
            return remove(index);
        };
        auto callback = std::make_shared<Callback>(unsubscriber, onDataReceived, onCompleted);
        return insertImpl(callback, index);
    }

    std::shared_ptr<Callback> CallbacksMap::insert(const OnMessageReceived &onMessageReceived, const onCompleted &onCompleted)
    {
        const std::lock_guard<std::mutex> lock(_guard);
        auto index = currentIndex++;
        auto unsubscriber = [this, index]() {
            return remove(index);
        };
        auto callback = std::make_shared<Callback>(unsubscriber, onMessageReceived, onCompleted);
        return insertImpl(callback, index);
    }

    std::shared_ptr<Callback> CallbacksMap::insertImpl(std::shared_ptr<Callback> callback, size_t index)
    {
        auto inserted = _map.insert(std::make_pair(index, callback));
        if (inserted.second)
        {
            return inserted.first->second;
        }
        return nullptr;
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
            pair.second->complete();
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
}