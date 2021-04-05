#pragma once
#include <iostream>
#include <map>
#include "IUnsubscribable.hpp"
#include "Control.hpp"
#include "IReadOnlySubject.hpp"

namespace pm {
    class Callback;
    using CallbacksMap = std::map<size_t, std::shared_ptr<Callback>>;

    class Callback final: public IUnsubscribable {
        public:
            bool unsunscribe();
            Control next(const std::string& msg) { return _next(msg); }
            void complete() { return _complete(); }
            Callback(std::shared_ptr<CallbacksMap> map, size_t identifier, const Next& next, const Complete complete);
        private:
            size_t _identifier;
            Next _next;
            Complete _complete;
            std::shared_ptr<CallbacksMap> _map;
    };
}