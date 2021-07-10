

set(SRC 
    NamedPipe.cpp
    ReadOnlyChannel.cpp
    WriteOnlyChannel.cpp
    Callback.cpp
    CallbacksHandler.cpp
    Unsubscriber.cpp
    CommandChannel.cpp
    CommandOutputChannel.cpp
)

if(OS_WINDOWS)

    set(PLATFORM_INCLUDES
        "Windows/.h"
    )

    set(PLATFORM_SRC
        #Windows/SocketServerWin.cpp
        #Windows/SocketClientWin.cpp
       # Windows/StdComunicatorWin.cpp
    )

elseif(OS_LINUX OR OS_FREEBSD OR OS_OPENBSD OR OS_NETBSD)
    
    set(PLATFORM_INCLUDES
        "Linux/.h"
    )

    set(PLATFORM_SRC
        Linux/SocketServerLinux.cpp
    )

endif()

add_library(SharpChannel
    ${SRC} 
    ${PLATFORM_SRC}
    SharpChannel.cpp
)

target_include_directories(SharpChannel PRIVATE
    ".h"
    ${PLATFORM_INCLUDES}
)