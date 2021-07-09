#include "SharpChannel.hpp"
#include "RunCommand.hpp"
#include "SocketServer.hpp"
#include "SocketClient.hpp"
namespace cm
{

    // settingsRunCommandChannel
    //std::unique_ptr<IReadOnlyChannel> SharpChannel::makeStdIO(const StdComunicatorSettings &settings, IChannelEventLoop *eventLoop)
    //{
    //    return nullptr; //return StdComunicator::getObject(settings, eventLoop);
    //}
//
    RunCommand::Ptr SharpChannel::makeSystemCommand(const RunCommandSettings &settings, IChannelEventLoop *eventLoop)
    {
        return RunCommand::create(settings, eventLoop);
    }
    //    
    //std::unique_ptr<IChannel> SharpChannel::makeSocketServer(const SocketServerSettings &settings, IChannelEventLoop *eventLoop)
    //{
    //    return SocketServer::getObject(settings, eventLoop);
    //}
//
    //std::unique_ptr<IChannel> SharpChannel::makeSocketClient(const SocketClientSettings &settings, IChannelEventLoop *eventLoop)
    //{
    //    return SocketClient::getObject(settings, eventLoop);
    //}
}