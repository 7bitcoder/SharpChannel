#pragma once
#include "IProcessComunicator.hpp"
#include "Settings.hpp"
#include "Control.hpp"
#include "Subject.hpp"

namespace pm {
    class NamedPipe final: public pm::Subject {
        public:
            NamedPipe(const NamedPipeSettings& settings) { _settings = settings; }
            ~NamedPipe() {}

            void run() override;
            bool next(const std::string& msg) override;
        private:
            NamedPipeSettings _settings;
    };
}