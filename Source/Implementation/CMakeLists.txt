
set(SRC 
    NamedPipe.cpp
    RunCommand.cpp
)

if(OS_WINDOWS)

    set(INCLUDES
        "Windows/.h"
    )

    set(PLATFORM_SRC
        Windows/SocketServerWin.cpp
        Windows/StdComunicatorWin.cpp
    )

elseif(OS_LINUX OR OS_FREEBSD OR OS_OPENBSD OR OS_NETBSD)
    
    set(INCLUDES
        "Linux/.h"
    )

    set(PLATFORM_SRC
        Linux/SocketServerLinux.cpp
    )

endif()

add_library(Implementation
    ${SRC} ${PLATFORM_SRC}
)

target_include_directories(Implementation PUBLIC
    "../Channels/.h"
    "../.h"
    ".h"
    ${INCLUDES}
)