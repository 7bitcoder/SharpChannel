#pragma once
#include <iostream>
#include <functional>
#include <map>
#include <mutex>
#include "Callback.hpp"
#include "IReadOnlySubject.hpp"

namespace pm {
    class ReadOnlySubject: public virtual IReadOnlySubject {
        public:
            ReadOnlySubject() { _callbacks = std::make_shared<CallbacksMap>(); }

            std::shared_ptr<IUnsubscribable> subscribe(const OnReceive& callback) final;

            std::shared_ptr<IUnsubscribable> subscribe(const OnReceive& callback, const OnComplete& complete) final;
            std::shared_ptr<IUnsubscribable> subscribe(IObserver& observer) final;
            
            virtual ~ReadOnlySubject() {}
        protected:
            void completeAll();
            void nextAll(const std::string& msg);
            CallbacksMap& getCallbacks() { return *_callbacks; }
        private:
            std::shared_ptr<CallbacksMap> _callbacks;
            size_t _indexCnt = 0;
    };
}