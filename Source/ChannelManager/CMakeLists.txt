
add_subdirectory(Channels)
add_subdirectory(ChannelsImplementation)

add_library(ChannelManager
    ChannelManager.cpp
)

target_link_libraries(ChannelManager
    Channels
)

target_include_directories(ChannelManager PUBLIC 
    ".h"
    "Channels/.h/.i"
    "Channels/.h"
    "ChannelsImplementation/.h"
)