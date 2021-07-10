#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include "Channels/IChannelEventLoop.hpp"

namespace cm
{
    class ChannelEventLoop final : public IChannelEventLoop
    {
    private:
        std::mutex _guard;
        std::condition_variable _cond;
        std::queue<std::function<void(void)>> _eventQueue;
        bool isCompleted = false;

    public:
        bool WaitForEventAndRun()
        {
            std::unique_lock<std::mutex> mlock(_guard);
            while (_eventQueue.empty())
            {
                _cond.wait(mlock);
            }
            auto &event = _eventQueue.front();
            if (event) {
                event();
                _eventQueue.pop();
                return true;
            } else {
                _eventQueue.pop();
                return false;
            }
        }

        void postChannelEvent(const std::function<void(void)> event) final
        {
            std::unique_lock<std::mutex> mlock(_guard);
            isCompleted = true;
            _eventQueue.push(event);
            mlock.unlock();     // unlock before notificiation to minimize mutex con
            _cond.notify_one(); // notify one waiting thread
        }
    };
}