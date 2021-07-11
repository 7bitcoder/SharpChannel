#include <iostream>
#include <thread>
#include <gtest/gtest.h>
#include "Config.hpp"
#include "SharpChannel.hpp"

#if defined(SYSTEM_WINDOWS)
std::string python = "python";
#elif defined(SYSTEM_LINUX)
std::string python = "python3";
#endif

namespace
{
    std::string tester = "echo.py";

    enum TestScenario
    {
        SimplePong = 1,
        MessageReceive
    };

    std::string getCmd(std::string echoStr)
    {
        return python + " " + tester + " " + echoStr;
    }
}

TEST(CommandOutputChannelTest, Normal)
{
    cm::RunCommandSettings settings;
    settings.command = " ";
    auto comunicator = cm::CommandOutputChannel::create(settings);

    auto onMessageReceived = [](const std::string &msg)
    {
        EXPECT_TRUE(msg.empty());
    };

    comunicator->subscribe(onMessageReceived);
    comunicator->run();
}

TEST(CommandOutputChannelTest, EchoTest)
{
    cm::RunCommandSettings settings;
    std::string echoMsg = "echoMessage";
    settings.command = getCmd(echoMsg);
    auto comunicator = cm::CommandOutputChannel::create(settings);

    auto onMessageReceived = [&echoMsg](const std::string &msg)
    {
        EXPECT_EQ(echoMsg, msg);
    };

    comunicator->subscribe(onMessageReceived);
    comunicator->run();
}
