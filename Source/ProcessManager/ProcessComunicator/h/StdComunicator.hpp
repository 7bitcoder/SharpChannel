#pragma once
#include "ProcessComunicator.hpp"
#include "Control.hpp"
#include "ReadOnlySubject.hpp"

namespace pc {
    class StdComunicator final: public pm::ReadOnlySubject {
        public:
            StdComunicator(const std::string& childProcess) { _childProcessCmd = childProcess; }
            ~StdComunicator() {}

            void run() override;
        private:
            std::string _childProcessCmd;
    };
}