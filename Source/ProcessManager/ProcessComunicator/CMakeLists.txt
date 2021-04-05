
add_library(ProcessComunicator
    StdComunicatorWin.cpp
    RunCommand.cpp
    NamedPipe.cpp
    SocketServer.cpp
)

target_include_directories(ProcessComunicator PUBLIC 
    "h"
    "../h"
    "../h/i"
)