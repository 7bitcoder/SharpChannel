#pragma once
#include <iostream>
#include <functional>
#include <map>
#include <thread>
#include <mutex>
#include "IWriteOnlySubject.hpp"

namespace pm {
    class WriteOnlySubject: public virtual IWriteOnlySubject {
        public:
            bool send(const std::string& msg) final {
                const std::lock_guard<std::mutex> lock(guard);
                return sendImpl(msg);
            }

            virtual bool sendImpl(const std::string& msg) = 0;
            virtual ~WriteOnlySubject() {}
        private:
            std::mutex guard;
    };
}