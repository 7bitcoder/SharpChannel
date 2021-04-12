#include "SharpChannel.hpp"
#include "RunCommand.hpp"
#include "SocketServer.hpp"
namespace cm
{

    // settingsRunCommandChannel
    std::unique_ptr<IReadOnlyChannel> SharpChannel::makeStdIO(const StdComunicatorSettings &settings, IChannelEventLoop *eventLoop)
    {
        return nullptr; //return StdComunicator::getObject(settings, eventLoop);
    }

    std::unique_ptr<IReadOnlyChannel> SharpChannel::makeSystemCommand(const RunCommandSettings &settings, IChannelEventLoop *eventLoop)
    {
        return RunCommand::getObject(settings, eventLoop);
    }

    std::unique_ptr<IChannel> SharpChannel::makeSocketServer(const SocketServerSettings &settings, IChannelEventLoop *eventLoop)
    {
        return SocketServer::getObject(settings, eventLoop);
    }
}