
add_library(ProcessComunicator
    StdComunicatorWin.cpp
)

target_include_directories(ProcessComunicator PUBLIC 
    "h"
    "../h"
)