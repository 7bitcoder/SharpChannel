#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include "CommandOutputChannel.hpp"
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
        auto comunicator = std::make_unique<CommandOutputChannel>(settings);
        comunicator->setChannelEventLoop(eventLoop);
        return comunicator;
    }

    void CommandOutputChannel::run()
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

    void CommandOutputChannel::finish()
    {
    }
}