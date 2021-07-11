
set(INCLUDES 
    ".h"
)

set(SRC 
    ReadOnlyChannel.cpp
    WriteOnlyChannel.cpp
    Callback.cpp
    CallbacksHandler.cpp
    Unsubscriber.cpp
    CommandChannel.cpp
    CommandOutputChannel.cpp
)
    
set(MULTIPLATFORM_INCLUDES
    "Multiplatform/.h"
)

set(MULTIPLATFORM
    Multiplatform/CommandChannelMulti.cpp
)

if(OS_WINDOWS)

    set(PLATFORM_INCLUDES
        "Windows/.h"
    )

    set(PLATFORM_SRC
        Windows/CommandOutputChannelWin.cpp
        #Windows/SocketServerWin.cpp
        Windows/SocketClientChannelWin.cpp
        #Windows/StdComunicatorWin.cpp
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
    ${MULTIPLATFORM}
    ${PLATFORM_SRC}
)

target_include_directories(SharpChannel PRIVATE
    ${INCLUDES}
    ${MULTIPLATFORM_INCLUDES}
    ${PLATFORM_INCLUDES}
)