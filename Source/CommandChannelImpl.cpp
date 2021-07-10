#include <string>
#include "CommandChannelImpl.hpp"
#include "ChannelException.hpp"

namespace cm
{

    CommandChannel::Ptr CommandChannel::create(const RunCommandSettings &settings, IChannelEventLoop *eventLoop)
    {
        auto comunicator = std::make_unique<CommandChannelImpl>(settings);
        comunicator->setChannelEventLoop(eventLoop);
        return comunicator;
    }

    void CommandChannelImpl::run()
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

    void CommandChannelImpl::finish()
    {
    }
}