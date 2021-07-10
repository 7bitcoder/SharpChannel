#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include "CommandOutputChannelImpl.hpp"
#include "ChannelException.hpp"

namespace cm
{
    namespace
    {
        std::string exec(const char *cmd)
        {
            std::array<char, 128> buffer;
            std::string result;
            std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd, "r"), _pclose);
            if (!pipe)
            {
                throw std::runtime_error("popen() failed!");
            }
            while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
            {
                result += buffer.data();
            }
            return result;
        }
    }

    CommandOutputChannel::Ptr CommandOutputChannel::create(const RunCommandSettings &settings, IChannelEventLoop *eventLoop)
    {
        auto comunicator = std::make_unique<CommandOutputChannelImpl>(settings);
        comunicator->setChannelEventLoop(eventLoop);
        return comunicator;
    }

    void CommandOutputChannelImpl::run()
    {
        std::string result;
        try
        {
            auto result = exec(_settings.command.c_str());
            nextAll(result);
        }
        catch (std::exception &e)
        {
            errorAll(e);
            throw;
        }
        catch (...)
        {
            ChannelException e("generic Error ocured");
            errorAll(e);
            throw e;
        }
        completeAll();
    }

    void CommandOutputChannelImpl::finish()
    {
    }
}