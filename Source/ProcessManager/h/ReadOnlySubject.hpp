#pragma once
#include <iostream>
#include <functional>
#include <map>
#include "Callback.hpp"
#include "IReadOnlySubject.hpp"

namespace pm {
    class ReadOnlySubject: public virtual IReadOnlySubject {
        public:
            ReadOnlySubject() { _callbacks = std::make_shared<CallbacksMap>(); }

            std::shared_ptr<IUnsubscribable> subscribe(const Next& callback) override;
            std::shared_ptr<IUnsubscribable> subscribe(const Next& callback, const Complete& complete) override;
            std::shared_ptr<IUnsubscribable> subscribe(IObserver& observer) override;
            
            virtual ~ReadOnlySubject() {}
        protected:
            void completeAll();
            void nextAll(const std::string& msg);
            std::shared_ptr<CallbacksMap> _callbacks;
        private:
            size_t _indexCnt = 0;
    };
}