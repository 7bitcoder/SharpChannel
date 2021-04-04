#pragma once
#include <iostream>
#include <functional>
#include <map>
#include "Control.hpp"
#include "ProcessComunicator.hpp"

namespace pm {
    class ReadOnlySubject;
    class Callback;

    using callback = std::function<Control(const std::string& msg)>;
    using callbacksMap = std::map<size_t, std::shared_ptr<Callback>>;
    
    struct Unsubscribable {
        virtual bool unsunscribe() = 0;
    };

    class Callback final: public Unsubscribable {
        friend class ReadOnlySubject;
        public:
            bool unsunscribe();
            Control call(const std::string& msg) { return _callback(msg); }
        private:
            Callback(std::shared_ptr<callbacksMap> map, size_t identifier, const callback& callback);
            size_t _identifier;
            callback _callback;
            std::shared_ptr<callbacksMap> _map;
    };
    
    struct ReadOnlySubject: public virtual ProcessComunicator {
        friend class Callback;
            ReadOnlySubject() { _callbacks = std::make_shared<callbacksMap>(); }
            virtual ~ReadOnlySubject() {}
            std::shared_ptr<Unsubscribable> subscribe(const callback& callback);
        protected:
            std::shared_ptr<callbacksMap> _callbacks;
        private:
            size_t _indexCnt = 0;
    };
}