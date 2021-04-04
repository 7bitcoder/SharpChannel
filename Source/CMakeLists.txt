add_subdirectory(ProcessManager)

add_executable(pipe main.cpp)

target_link_libraries(pipe
    ProcessManager
)

target_include_directories(pipe PUBLIC 
    "ProcessManager/h"
)