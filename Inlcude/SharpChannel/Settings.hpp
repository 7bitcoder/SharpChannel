#pragma once
#include <iostream>
#include <string>

namespace cm
{
    struct StdComunicatorSettings
    {
        std::string childProcessCommand;
    };

    struct RunCommandSettings
    {
        std::string command = "";
        bool captureOutput = false;
    };

    struct NamedPipeSettings
    {
        std::string pipeName;
        std::string subProcessCommand;
    };

    struct SocketServerSettings
    {
        enum class SocketType
        {
            Tcp,
            Udp
        };
        SocketType socketType = SocketType::Tcp;
        size_t port = 0; // chose default port
        size_t receiveBufferSize = 4096;
        size_t sendBufferSize = 4096;
    };
}