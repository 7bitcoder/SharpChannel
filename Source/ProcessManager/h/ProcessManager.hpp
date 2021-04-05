#pragma once
#include <iostream>
#include <functional>
#include <memory>
#include "IProcessComunicator.hpp"
#include "StdComunicator.hpp"
#include "SocketServer.hpp"
#include "Settings.hpp"

#ifdef WIN
    
#endif

namespace pm {
    class ProcessManagerException: public std::exception {
        public:
            explicit ProcessManagerException(const char* message)
                : msg_(message) {}

            explicit ProcessManagerException(const std::string& message)
                : msg_(message) {}

            virtual ~ProcessManagerException() noexcept {}

            virtual const char* what() const noexcept { return msg_.c_str(); }

        protected:
            std::string msg_;
    };
    
    class ProcessManager {
        public:
            static std::unique_ptr<IReadOnlySubject> makeStdIO(const StdSubjectSettings& settings);

            static std::unique_ptr<IReadOnlySubject> makeSystemCommand(const RunCommandSettings& settings);

            static std::unique_ptr<ISubject> makeSocketServer(const SocketServerSettings& settings);
    };
}