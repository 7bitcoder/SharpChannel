#include <string>
#include "NamedPipe.hpp"
#include "OsFlags.hpp"

#ifdef WIN

#include <windows.h> 
#include <stdio.h>
#include <conio.h>
#include <tchar.h>

#define BUFSIZE 512

namespace cm {

    void NamedPipe::run() {
       
    }

    bool NamedPipe::sendImpl(const std::string& msg) {
        return false;
    }
    
    bool NamedPipe::sendImpl(const char* data, size_t lenght) {
        return false;
    }
}
#endif