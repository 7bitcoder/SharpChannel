#include "SharpChannel.hpp"
#include "StdComunicator.hpp"
#include "RunCommand.hpp"
#include "SocketServer.hpp"
namespace cm {

    // settingsRunCommandChannel
    std::unique_ptr<IReadOnlyChannel> SharpChannel::makeStdIO(const StdChannelSettings& settings ) {
        return std::make_unique<StdComunicator>(settings);
    }

    std::unique_ptr<IReadOnlyChannel> SharpChannel::makeSystemCommand(const RunCommandSettings& settings ) {
        return std::make_unique<RunCommand>(settings);
    }

    std::unique_ptr<IChannel> SharpChannel::makeSocketServer(const SocketServerSettings& settings) {
        return std::make_unique<SocketServer>(settings);
    }
}