#pragma once
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
   //define something for Windows (32-bit and 64-bit, this part is common)
   #define WIN
   #ifdef _WIN64
        #define WIN64
   #else
        #define WIN32
   #endif
#elif __APPLE__
    #include <TargetConditionals.h>
    #define APPLE
    #if TARGET_IPHONE_SIMULATOR
        #define IOS
    #elif TARGET_OS_IPHONE
        #define IOS
    #elif TARGET_OS_MAC
        #define MAC
    #else
    #   error "Unknown Apple platform"
    #endif
#elif __linux__
    #define LINUX
#elif __unix__ // all unices not caught above
    #define LINUX
    // Unix
#elif defined(_POSIX_VERSION)
    #define LINUX
    // POSIX
#else
#   error "Unknown compiler"
#endif