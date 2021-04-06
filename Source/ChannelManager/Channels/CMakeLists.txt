

add_library(Channels
    ReadOnlyChannel.cpp
    WriteOnlyChannel.cpp
    Callback.cpp
)

target_link_libraries(Channels
    ChannelsImplementation
)

target_include_directories(Channels PUBLIC 
    ".h"
    ".h/.i"
)