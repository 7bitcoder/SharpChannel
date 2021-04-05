#include <utility>
#include "ReadOnlySubject.hpp"
#include "Callback.hpp"
#include "ProcessManager.hpp"

namespace pm {

    Callback::Callback(std::shared_ptr<CallbacksMap> map, size_t identifier, const OnReceive& onReceive, const OnComplete& onComplete)
        : _map(map), _onReceive(std::move(onReceive)), _onComplete(std::move(onComplete)) { _identifier = identifier; }

    bool Callback::unsunscribe() {
        if(_map) {
            return _map->erase(_identifier) == 1;
        }
        return false;
    }
}