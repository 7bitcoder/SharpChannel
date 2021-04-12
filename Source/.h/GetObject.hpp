#pragma once
#include <memory>

namespace cm
{
    template <class T, class S>
    class GetObject
    {
        static std::unique_ptr<T> getObject(const S &);
    };
}