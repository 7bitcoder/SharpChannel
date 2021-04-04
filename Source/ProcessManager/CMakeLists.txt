
add_subdirectory(ProcessComunicator)

add_library(ProcessManager
    ProcessManager.cpp
    ReadOnlySubject.cpp
)

target_link_libraries(ProcessManager
    ProcessComunicator
)

target_include_directories(ProcessManager PUBLIC 
    "h"
    "ProcessComunicator/h"
)