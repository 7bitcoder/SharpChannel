
add_library(ChannelsImplementation
    StdComunicatorWin.cpp
    RunCommand.cpp
    NamedPipe.cpp
    SocketServer.cpp
)

target_include_directories(ChannelsImplementation PUBLIC 
    ".h"
    "../Channels/.h"
    "../Channels/.h/.i"
)