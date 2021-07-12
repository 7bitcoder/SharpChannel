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
    class CommandOutputChannelTest : public ::testing::Test
    {
    protected:
        CommandOutputChannelTest() {}

        void SetUp() override
        {
            cm::RunCommandSettings settings;
            echoMsg = "echoMessage";
            settings.command = getCmd(echoMsg);
            comunicator = cm::CommandOutputChannel::create(settings);
        }

        void TearDown() override {}

        ~CommandOutputChannelTest() {}

        static void TearDownTestSuite() {}

        std::string getCmd(std::string echoStr)
        {
            return python + " " + tester + " " + echoStr;
        }

        std::string tester = "echo.py";
        std::string echoMsg;
        cm::CommandOutputChannel::Ptr comunicator;
    };

}

TEST_F(CommandOutputChannelTest, Normal)
{
    bool messageArrived = false;
    std::string message;

    auto onMessageReceived = [&messageArrived, &message](const std::string &msg)
    {
        messageArrived = true;
        message = msg;
    };

    comunicator->subscribe(onMessageReceived);
    comunicator->run();

    EXPECT_EQ(message, echoMsg);
}
