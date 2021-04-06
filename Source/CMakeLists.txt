
add_subdirectory(Channels)

add_library(SharpChannel
    SharpChannel.cpp
)

target_link_libraries(SharpChannel
    Channels
)