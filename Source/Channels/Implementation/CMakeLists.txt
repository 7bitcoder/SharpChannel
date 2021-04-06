

add_library(Implementation
    NamedPipe.cpp
    RunCommand.cpp
    SocketServer.cpp
    StdComunicatorWin.cpp
)

target_include_directories(Implementation PUBLIC
    "../.h"
    ".h"
)