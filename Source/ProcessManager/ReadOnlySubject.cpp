#include <utility>
#include <functional>
#include "ReadOnlySubject.hpp"
#include "ProcessManager.hpp"
#include "IObserver.hpp"

namespace pm {

    std::shared_ptr<IUnsubscribable> ReadOnlySubject::subscribe(const Next& next) {
        return subscribe(next, [](){});
    }

    std::shared_ptr<IUnsubscribable> ReadOnlySubject::subscribe(const Next& next, const Complete& complete) {
        auto index = _indexCnt++;
        std::shared_ptr<Callback> c( new Callback(_callbacks, index, next, complete));
        auto inserted = _callbacks->insert(std::make_pair(index, c));
        if(inserted.second) {
            return inserted.first->second;
        }
        return nullptr;
    }

    void ReadOnlySubject::nextAll(const std::string& msg) {
        for(auto& pair: *_callbacks) {
            pair.second->next(msg);
        }
    } 
    
    void ReadOnlySubject::completeAll() {
        for(auto& pair: *_callbacks) {
            pair.second->complete();
        }
    } 

    std::shared_ptr<IUnsubscribable> ReadOnlySubject::subscribe(IObserver& observer) {
        return subscribe(std::bind(&IObserver::next, &observer, std::placeholders::_1), std::bind(&IObserver::complete, &observer));
    }
}