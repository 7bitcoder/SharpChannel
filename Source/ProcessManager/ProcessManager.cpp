#include "ProcessManager.hpp"
#include "StdComunicator.hpp"
#include "RunCommand.hpp"
#include "SocketServer.hpp"
namespace pm {

    // settingsRunCommandSubject
    std::unique_ptr<IReadOnlySubject> ProcessManager::makeStdIO(const StdSubjectSettings& settings ) {
        return std::make_unique<StdComunicator>(settings);
    }

    std::unique_ptr<IReadOnlySubject> ProcessManager::makeSystemCommand(const RunCommandSettings& settings ) {
        return std::make_unique<RunCommand>(settings);
    }

    std::unique_ptr<ISubject> ProcessManager::makeSocketServer(const SocketServerSettings& settings) {
        return std::make_unique<SocketServer>(settings);
    }
}