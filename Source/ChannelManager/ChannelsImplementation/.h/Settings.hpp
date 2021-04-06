#pragma once
#include <iostream>

namespace cm {
    struct StdChannelSettings {
        std::string childProcessCommand;
    };

    struct RunCommandSettings {
        std::string command;
    };

    struct  NamedPipeSettings
    {
        std::string pipeName;
        std::string subProcessCommand;
    };
    
    struct SocketServerSettings {
        enum class SocketType {
            Tcp,
            Udp
        };
        SocketType socketType = SocketType::Tcp;
        size_t port = 0; // chose default port
        size_t receiveBufferSize = 4096;
        size_t sendBufferSize = 4096;
    };
}