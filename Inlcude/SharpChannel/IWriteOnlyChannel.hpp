#pragma once
#include <vector>
#include "IRunnable.hpp"

namespace cm
{

    struct IWriteOnlyChannel : public virtual IRunnable
    {
        virtual bool sendMessage(const std::string &msg) = 0;
        virtual bool sendData(const std::vector<char> &data) = 0;

        virtual ~IWriteOnlyChannel() {}
    };
}