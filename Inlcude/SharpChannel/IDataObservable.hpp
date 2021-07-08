#pragma once
#include <memory>
#include <vector>
#include <functional>
#include"IUnsubscribable.hpp"

namespace cm
{
    using OnDataReceived = std::function<void(const std::vector<char> &)>;

    struct IDataObservable
    {
        virtual std::unique_ptr<IUnsubscribable> subscribe(const OnDataReceived &onDataReceived) = 0;

        virtual ~IDataObservable() {}
    };
}