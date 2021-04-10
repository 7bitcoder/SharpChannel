#pragma once
#include <iostream>
#include <vector>
#include "IObserver.hpp"

namespace cm
{

    struct IDataObserver : public virtual IObserver
    {
        virtual void onDataReceived(const std::vector<char> &data) = 0;

        virtual ~IDataObserver() {}
    };
}