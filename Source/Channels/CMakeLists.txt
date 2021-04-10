

add_library(Channels
    ReadOnlyChannel.cpp
    WriteOnlyChannel.cpp
    Callback.cpp
    CallbacksMap.cpp
    Unsubscriber.cpp
)

target_include_directories(Channels PUBLIC
    ".h"
)