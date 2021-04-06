#include "WriteOnlyChannel.hpp"

namespace cm {

    bool WriteOnlyChannel::send(const std::string& msg) {
        const std::lock_guard<std::mutex> lock(guard);
        return sendImpl(msg);
    }

    bool WriteOnlyChannel::send(const char* data, size_t lenght) {
        const std::lock_guard<std::mutex> lock(guard);
        return sendImpl(data, lenght);
    }
}