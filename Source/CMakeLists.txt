
add_subdirectory(Channels)
add_subdirectory(Implementation)

add_library(SharpChannel
    SharpChannel.cpp
)

target_link_libraries(SharpChannel
    Channels
    Implementation
)