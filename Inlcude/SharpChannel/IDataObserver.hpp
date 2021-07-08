#pragma once
#include <iostream>
#include <vector>

namespace cm
{

    struct IDataObserver 
    {
        virtual bool next(const std::vector<char> &data) = 0;

        virtual ~IDataObserver() {}
    };
}