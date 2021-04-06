#pragma once
#include <iostream>
#include <functional>
#include <map>
#include <thread>
#include <mutex>
#include "IWriteOnlyChannel.hpp"

namespace cm {
    class WriteOnlyChannel: public virtual IWriteOnlyChannel {
        public:
            bool send(const std::string& msg) final;
            bool send(const char* data, size_t lenght) final;

            virtual bool sendImpl(const std::string& msg) = 0;
            virtual bool sendImpl(const char* data, size_t lenght) = 0;

            virtual ~WriteOnlyChannel() {}
        private:
            std::mutex guard;
    };
}