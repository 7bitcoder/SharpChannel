
add_subdirectory(ProcessComunicator)

add_library(ProcessManager
    ProcessManager.cpp
    ReadOnlySubject.cpp
    Callback.cpp
)

target_link_libraries(ProcessManager
    ProcessComunicator
)

target_include_directories(ProcessManager PUBLIC 
    "h"
    "h/i"
    "ProcessComunicator/h"
)