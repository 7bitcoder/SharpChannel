
add_subdirectory(Implementation)

add_library(Channels
    ReadOnlyChannel.cpp
    WriteOnlyChannel.cpp
    Callback.cpp
)

target_link_libraries(Channels
    Implementation
)

target_include_directories(Channels PUBLIC
    ".h"
)