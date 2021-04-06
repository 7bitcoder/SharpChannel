#pragma once

namespace cm {

    struct IRunnable {
        virtual void run() = 0;
        virtual void finish() {};
        
        virtual ~IRunnable() {}
    };
}