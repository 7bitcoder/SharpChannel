#include <string>
#include "RunCommand.hpp"

namespace cm {

    std::unique_ptr<RunCommand> RunCommand::getObject(const RunCommandSettings& settings) {
        return std::make_unique<RunCommand>(settings);
    }

    void RunCommand::run() {
        auto result = std::system(_settings.command.c_str());
        auto strRes = std::to_string(result);
        nextAll(strRes);
        completeAll();
    }

    void RunCommand::finish() {

    }
}