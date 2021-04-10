#include <string>
#include "RunCommand.hpp"

namespace cm
{

    std::unique_ptr<RunCommand> RunCommand::getObject(const RunCommandSettings &settings, IChannelEventLoop *eventLoop)
    {
        auto comunicator = std::make_unique<RunCommand>(settings);
        comunicator->setChannelEventLoop(eventLoop);
        return comunicator;
    }

    void RunCommand::run()
    {
        try {
            auto result = std::system(_settings.command.c_str());
            auto strRes = std::to_string(result);
            nextAll(strRes);
            completeAll();
        } catch (std::exception& e) {
            errorAll(e);
        }
    }

    void RunCommand::finish()
    {
    }
}