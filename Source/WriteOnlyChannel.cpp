#include "WriteOnlyChannel.hpp"

namespace cm
{

    bool WriteOnlyChannel::sendMessage(const std::string &msg)
    {
        const std::lock_guard<std::mutex> lock(_guard);
        return sendMessageImpl(msg);
    }

    bool WriteOnlyChannel::sendData(const std::vector<char> &data)
    {
        const std::lock_guard<std::mutex> lock(_guard);
        return sendDataImpl(data);
    }
}