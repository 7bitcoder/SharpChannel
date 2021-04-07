

add_library(Channels
    ReadOnlyChannel.cpp
    WriteOnlyChannel.cpp
    Callback.cpp
)

target_include_directories(Channels PUBLIC
    ".h"
)