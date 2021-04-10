#pragma once
#include <iostream>
#include <vector>
#include "Control.hpp"
#include "IObserver.hpp"

namespace cm
{

    struct IDataObserver : public virtual IObserver
    {
        virtual Control onDataReceived(const std::vector<char> &data) = 0;

        virtual ~IDataObserver() {}
    };
}