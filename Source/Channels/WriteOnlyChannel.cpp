#include "WriteOnlyChannel.hpp"

namespace cm {

    bool WriteOnlyChannel::sendMessage(const std::string& msg) {
        const std::lock_guard<std::mutex> lock(guard);
        return sendMessageImpl(msg);
    }

    bool WriteOnlyChannel::sendData(const char* data, size_t lenght) {
        const std::lock_guard<std::mutex> lock(guard);
        return sendDataImpl(data, lenght);
    }
}