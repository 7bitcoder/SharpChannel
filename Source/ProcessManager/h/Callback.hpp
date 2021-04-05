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
            Control next(const std::string& msg) { return _onReceive(msg); }
            void complete() { return _onComplete(); }
            Callback(std::shared_ptr<CallbacksMap> map, size_t identifier, const OnReceive& onReceive, const OnComplete& onComplete);
        private:
            size_t _identifier;
            OnReceive _onReceive;
            OnComplete _onComplete;
            std::shared_ptr<CallbacksMap> _map;
    };
}