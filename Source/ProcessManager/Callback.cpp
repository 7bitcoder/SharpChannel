#include <utility>
#include "ReadOnlySubject.hpp"
#include "Callback.hpp"
#include "ProcessManager.hpp"

namespace pm {

    Callback::Callback(std::shared_ptr<CallbacksMap> map, size_t identifier, const Next& next, const Complete complete)
        : _map(map), _next(std::move(next)), _complete(std::move(complete)) { _identifier = identifier; }

    bool Callback::unsunscribe() {
        if(_map) {
            return _map->erase(_identifier) == 1;
        }
        return false;
    }
}