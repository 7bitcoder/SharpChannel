#pragma once
#include <exception>

namespace cm
{

    struct IErrorObserver
    {
        virtual void error(const std::exception& e) = 0;

        virtual ~IErrorObserver() {}
    };
}