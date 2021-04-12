

set( ChannelsSrc 
    Channels/ReadOnlyChannel.cpp
    Channels/WriteOnlyChannel.cpp
    Channels/Callback.cpp
    Channels/CallbacksMap.cpp
    Channels/Unsubscriber.cpp
)

set(SRC 
    Implementation/NamedPipe.cpp
    Implementation/RunCommand.cpp
)

if(OS_WINDOWS)

    set(INCLUDES
        "Implementation/Windows/.h"
    )

    set(PLATFORM_SRC
        Implementation/Windows/SocketServerWin.cpp
        Implementation/Windows/StdComunicatorWin.cpp
    )

elseif(OS_LINUX OR OS_FREEBSD OR OS_OPENBSD OR OS_NETBSD)
    
    set(INCLUDES
        "Implementation/Linux/.h"
    )

    set(PLATFORM_SRC
        Implementation/Linux/SocketServerLinux.cpp
    )

endif()

add_library(SharpChannel
    ${ChannelsSrc}
    ${SRC} 
    ${PLATFORM_SRC}
    SharpChannel.cpp
)

target_include_directories(SharpChannel PUBLIC
    ".h"
    "Channels/.h"
    "Implementation/.h"
    ${INCLUDES}
)