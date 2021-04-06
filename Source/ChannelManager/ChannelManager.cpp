#include "ChannelManager.hpp"
#include "StdComunicator.hpp"
#include "RunCommand.hpp"
#include "SocketServer.hpp"
namespace cm {

    // settingsRunCommandChannel
    std::unique_ptr<IReadOnlyChannel> ChannelManager::makeStdIO(const StdChannelSettings& settings ) {
        return std::make_unique<StdComunicator>(settings);
    }

    std::unique_ptr<IReadOnlyChannel> ChannelManager::makeSystemCommand(const RunCommandSettings& settings ) {
        return std::make_unique<RunCommand>(settings);
    }

    std::unique_ptr<IChannel> ChannelManager::makeSocketServer(const SocketServerSettings& settings) {
        return std::make_unique<SocketServer>(settings);
    }
}