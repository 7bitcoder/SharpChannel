#pragma once
#include "IProcessComunicator.hpp"
#include "Settings.hpp"
#include "Control.hpp"
#include "ReadOnlySubject.hpp"

namespace pm {
    class StdComunicator final: public pm::ReadOnlySubject {
        public:
            StdComunicator(const StdSubjectSettings& settings) { _settings = settings; }
            ~StdComunicator() {}

            void run() override;
        private:
            StdSubjectSettings _settings;
    };
}