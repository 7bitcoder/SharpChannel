#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include "IChannelEventLoop.hpp"

namespace cm {
    class ChannelEventLoop final: public IChannelEventLoop
    {
    private:
        std::mutex _guard;
        std::condition_variable _cond;
        std::queue<std::function<void(void)>> _eventQueue;
    public:
        std::function<void()>& WaitForEventAndRun() {
            std::unique_lock<std::mutex> mlock(_guard);
            while (_eventQueue.empty())
            {
                _cond.wait(mlock);
            }
            auto& event = _eventQueue.front();
            event();
            _eventQueue.pop();
        }

        void postChannelEvent(const std::function<void(void)> event) final {
            std::unique_lock<std::mutex> mlock(_guard);
            _eventQueue.push(event);
            mlock.unlock();     // unlock before notificiation to minimize mutex con
            _cond.notify_one(); // notify one waiting thread
        }

        ChannelEventLoop() {};
        ~ChannelEventLoop() {};
    };
}