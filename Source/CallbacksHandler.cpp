#include "CallbacksHandler.hpp"

namespace cm
{
    CallbacksHandler::CallbackId CallbacksHandler::addCallback(Callback &callback)
    {
        const std::lock_guard<std::mutex> lock(_guard);
        const CallbackId index = ++currentIndex;
        auto inserted = _callbacksMap.emplace(index, std::move(callback));
        return inserted.second ? index : 0;
    }

    bool CallbacksHandler::removeCallback(CallbackId callbackId)
    {
        const std::lock_guard<std::mutex> lock(_guard);
        return _callbacksMap.erase(callbackId) == 1;
    }

    void CallbacksHandler::completeAll() const
    {
        const std::lock_guard<std::mutex> lock(_guard);
        for (auto &pair : _callbacksMap)
        {
            pair.second.onComplete();
        }
    }

    void CallbacksHandler::nextAll(const std::string &msg) const
    {
        const std::lock_guard<std::mutex> lock(_guard);
        std::unique_ptr<std::vector<char>> ptr;
        for (auto &pair : _callbacksMap)
        {
            auto &callback = pair.second;
            if (callback.isData())
            {
                if (!ptr)
                { // create if we need
                    ptr = std::make_unique<std::vector<char>>(msg.begin(), msg.end());
                }
                callback.onDataReceived(*ptr);
            }
            else if (callback.isMessage())
            {
                callback.onMessageReceived(msg);
            }
        }
    }

    void CallbacksHandler::nextAll(const std::vector<char> &data) const
    {
        const std::lock_guard<std::mutex> lock(_guard);
        std::unique_ptr<std::string> ptr;
        for (auto &pair : _callbacksMap)
        {
            auto &callback = pair.second;
            if (callback.isData())
            {
                callback.onDataReceived(data);
            }
            else if (callback.isMessage())
            {
                if (!ptr)
                { // create if we need
                    ptr = std::make_unique<std::string>(data.begin(), data.end());
                }
                callback.onMessageReceived(*ptr);
            }
        }
    }

    void CallbacksHandler::errorAll(const std::exception &error) const
    {
        const std::lock_guard<std::mutex> lock(_guard);
        for (auto &pair : _callbacksMap)
        {
            pair.second.onError(error);
        }
    }
}