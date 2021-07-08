#include "ChannelObserver.hpp"

namespace cm
{

    bool ChannelObserver::next(const std::string &msg)
    {
        const std::lock_guard<std::mutex> lock(_guard);
        return sendMessageImpl(msg);
    }

    bool ChannelObserver::next(const std::vector<char> &data)
    {
        const std::lock_guard<std::mutex> lock(_guard);
        return sendDataImpl(data);
    }
}