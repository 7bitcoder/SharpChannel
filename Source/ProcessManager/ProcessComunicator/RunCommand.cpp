#include <string>
#include "RunCommand.hpp"

namespace pm {

    void RunCommand::run() {
        auto result = std::system(_settings.command.c_str());
        auto strRes = std::to_string(result);
        nextAll(strRes);
        completeAll();
    }

    void RunCommand::finish() {

    }
}