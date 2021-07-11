#include "WriteOnlyChannel.hpp"

namespace cm
{

    bool WriteOnlyChannel::next(const std::string &msg)
    {
        const std::lock_guard<std::mutex> lock(_guard);
        return sendMessageImpl(msg);
    }

    bool WriteOnlyChannel::next(const std::vector<char> &data)
    {
        const std::lock_guard<std::mutex> lock(_guard);
        return sendDataImpl(data);
    }
}