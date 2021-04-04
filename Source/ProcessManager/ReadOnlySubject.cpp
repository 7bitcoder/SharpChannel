#include <utility>
#include "ReadOnlySubject.hpp"
#include "ProcessManager.hpp"

namespace pm {

    Callback::Callback(std::shared_ptr<callbacksMap> map, size_t identifier, const callback& callback)
        : _map(map), _callback(callback) { _identifier = identifier; }

    bool Callback::unsunscribe() {
        if(_map) {
            return _map->erase(_identifier) == 1;
        }
        return false;
    }

    std::shared_ptr<Unsubscribable> ReadOnlySubject::subscribe(const callback& callback) {
        auto index = _indexCnt++;
        std::shared_ptr<Callback> c( new Callback(_callbacks, index, callback));
        auto inserted = _callbacks->insert(std::make_pair(index, c));
        if(inserted.second) {
            return inserted.first->second;
        }
        return nullptr;
    }
}