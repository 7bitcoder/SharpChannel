#include "ProcessManager.hpp"
#include "StdComunicator.hpp"

namespace pm {

    // settings
    std::unique_ptr<ReadOnlySubject> ProcessManager::makeSTDComunicator(const std::string& childProcessCommand ) {
        return std::make_unique<pc::StdComunicator>(childProcessCommand);
    }
}