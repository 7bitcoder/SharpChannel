#include <memory>
#include <vector>
#include "NamedPipeChannelWin.hpp"
#include "NamedPipeChannel.hpp"
#include "ChannelException.hpp"
#include <windows.h>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")
#pragma comment(lib, "AdvApi32.lib")

namespace cm
{

    INamedPipeChannel::Ptr NamedPipeChannel::getImplementation(const NamedPipeChannelSettings &settings, IChannelEventLoop *eventLoop)
    {
        auto comunicator = std::make_unique<NamedPipeChannelWin>(settings);
        comunicator->setChannelEventLoop(eventLoop);
        return comunicator;
    }

    NamedPipeChannelWin::~NamedPipeChannelWin()
    {
    }

    void NamedPipeChannelWin::init() {}

    void NamedPipeChannelWin::run()
    {
    }

    bool NamedPipeChannelWin::sendDataImpl(const std::vector<char> &data)
    {
        return true;
    }

    bool NamedPipeChannelWin::sendMessageImpl(const std::string &msg)
    {
        return true;
    }
}