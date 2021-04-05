#pragma once
#include "IProcessComunicator.hpp"
#include "Settings.hpp"
#include "Control.hpp"
#include "ReadOnlySubject.hpp"

namespace pm {
    class RunCommand final: public pm::ReadOnlySubject {
        public:
            RunCommand(const RunCommandSettings& settings): _settings(std::move(settings)) {}
            ~RunCommand() {}

            void run() override;
            void finish() override;
        private:
            RunCommandSettings _settings;
    };
}