#include <utility>
#include <functional>
#include "ReadOnlySubject.hpp"
#include "ProcessManager.hpp"
#include "IObserver.hpp"

namespace pm {

    std::shared_ptr<IUnsubscribable> ReadOnlySubject::subscribe(const OnReceive& next) {
        return subscribe(next, [](){});
    }

    std::shared_ptr<IUnsubscribable> ReadOnlySubject::subscribe(const OnReceive& next, const OnComplete& complete) {
        auto index = _indexCnt++;
        std::shared_ptr<Callback> c( new Callback(_callbacks, index, next, complete));
        auto inserted = getCallbacks().insert(std::make_pair(index, c));
        if(inserted.second) {
            return inserted.first->second;
        }
        return nullptr;
    }

    std::shared_ptr<IUnsubscribable> ReadOnlySubject::subscribe(IObserver& observer) {
        return subscribe(std::bind(&IObserver::onReceive, &observer, std::placeholders::_1), std::bind(&IObserver::onComplete, &observer));
    }

    void ReadOnlySubject::nextAll(const std::string& msg) {
        for(auto& pair: getCallbacks()) {
            pair.second->next(msg);
        }
    } 
    
    void ReadOnlySubject::completeAll() {
        for(auto& pair: getCallbacks()) {
            pair.second->complete();
        }
    } 
}