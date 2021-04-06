
add_subdirectory(ChannelManager)

add_executable(pipe main.cpp)

target_link_libraries(pipe
    ChannelManager
)

target_include_directories(pipe PUBLIC 
    "ChannelManager/h"
)