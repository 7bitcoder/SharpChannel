#include <string>
#include "RunCommand.hpp"

namespace cm {

    std::unique_ptr<RunCommand> RunCommand::getObject(const RunCommandSettings& settings) {
        return std::make_unique<RunCommand>(settings);
    }

    void RunCommand::run() {
        std::streambuf* oldCoutStreamBuf;
        std::ostringstream strCout;
        if(_settings.captureOutput) {
            oldCoutStreamBuf = std::cout.rdbuf();
            std::cout.rdbuf(strCout.rdbuf());
        }

        auto result = std::system(_settings.command.c_str());
        auto strRes = std::to_string(result);

        if(_settings.captureOutput) {
            nextAll(strCout.str());
    	    std::cout.rdbuf(oldCoutStreamBuf);
        } else {
            nextAll(strRes);
        }
        completeAll();
    }

    void RunCommand::finish() {

    }
}