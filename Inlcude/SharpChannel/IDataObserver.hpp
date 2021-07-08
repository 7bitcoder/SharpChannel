#pragma once
#include <iostream>
#include <vector>

namespace cm
{

    struct IDataObserver 
    {
        virtual void next(const std::vector<char> &data) = 0;

        virtual ~IDataObserver() {}
    };
}