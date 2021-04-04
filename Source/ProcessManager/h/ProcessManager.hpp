#pragma once
#include <iostream>
#include <functional>
#include <memory>
#include "ProcessComunicator.hpp"
#include "StdComunicator.hpp"

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
            static std::unique_ptr<ReadOnlySubject> makeSTDComunicator(const std::string& childProcessCommand);
    };
}