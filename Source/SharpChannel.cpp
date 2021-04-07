#include "SharpChannel.hpp"
#include "StdComunicator.hpp"
#include "RunCommand.hpp"
#include "SocketServer.hpp"
namespace cm {

    // settingsRunCommandChannel
    std::unique_ptr<IReadOnlyChannel> SharpChannel::makeStdIO(const StdComunicatorSettings& settings ) {
        return StdComunicator::getObject(settings);
    }

    std::unique_ptr<IReadOnlyChannel> SharpChannel::makeSystemCommand(const RunCommandSettings& settings ) {
        return RunCommand::getObject(settings);
    }

    std::unique_ptr<IChannel> SharpChannel::makeSocketServer(const SocketServerSettings& settings) {
        return SocketServer::getObject(settings);
    }
}