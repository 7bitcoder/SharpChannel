#pragma once
#include "IObservers.hpp"

namespace cm
{
    struct IWriteOnlyChannel : public ICompleteObserver, public IErrorObserver, public IMessageObserver, public IDataObserver
    {
        using Ptr = std::shared_ptr<IWriteOnlyChannel>;

        bool next(const std::string &msg) override = 0;
        bool next(const std::vector<char> &data) override = 0;
        void error(const std::exception& e) override = 0;
        void complete() override = 0;
    };
}