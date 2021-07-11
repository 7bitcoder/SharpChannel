#include <string>
#include "CommandChannel.hpp"
#include "CommandChannelMulti.hpp"
#include "ChannelException.hpp"

namespace cm
{

    ICommandChannel::Ptr CommandChannel::getImplementation(const RunCommandSettings &settings, IChannelEventLoop *eventLoop)
    {
        auto comunicator = std::make_unique<CommandChannelMulti>(settings);
        comunicator->setChannelEventLoop(eventLoop);
        return comunicator;
    }

    void CommandChannelMulti::run()
    {
        std::string result;
        try {
            auto resultInt = std::system(_settings.command.c_str());
            result = std::to_string(resultInt);
            nextAll(result);
        } catch (std::exception& e) {
            errorAll(e);
            throw;
        } catch (...) {
            ChannelException e("generic Error ocured");
            errorAll(e);
            throw e;
        }
        completeAll();
    }

    void CommandChannelMulti::finish()
    {
    }
}