#pragma once
#include <iostream>
#include <vector>
#include <functional>
#include <map>
#include <thread>
#include <mutex>
#include "IWriteOnlyChannel.hpp"

namespace cm
{
    class WriteOnlyChannel : public virtual IWriteOnlyChannel
    {
    public:
        bool sendMessage(const std::string &msg) final;
        bool sendData(const std::vector<char> &data) final;

        virtual bool sendMessageImpl(const std::string &msg) = 0;
        virtual bool sendDataImpl(const std::vector<char> &data) = 0;

        ~WriteOnlyChannel() {}

    private:
        std::mutex _guard;
    };
}