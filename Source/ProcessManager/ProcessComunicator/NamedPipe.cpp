#include <string>
#include "NamedPipe.hpp"
#include "OsFlags.hpp"

#ifdef WIN

#include <windows.h> 
#include <stdio.h>
#include <conio.h>
#include <tchar.h>

#define BUFSIZE 512

namespace pm {

    void NamedPipe::run() {
       
    }

    bool NamedPipe::send(const std::string& msg) {
        return false;
    }
}
#endif