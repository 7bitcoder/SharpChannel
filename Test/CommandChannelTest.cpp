#include <iostream>
#include <thread>
#include <gtest/gtest.h>
#include "SharpChannel.hpp"


class CommandChannelTest : public ::testing::Test
{
protected:
    CommandChannelTest() {}

    void SetUp() override
    {
        cm::RunCommandSettings settings;
        settings.command = " ";
        comunicator = cm::CommandChannel::create(settings);
    }

    void TearDown() override {}

    ~CommandChannelTest() {}

    static void TearDownTestSuite() {}

    cm::CommandChannel::Ptr comunicator;
};

TEST_F(CommandChannelTest, Normal)
{
    bool messageArrived = false;
    std::string message;

    auto onMessageReceived = [&messageArrived, &message](const std::string &msg) {
        messageArrived = true;
        message = msg;
    };

    comunicator->subscribe(onMessageReceived);
    comunicator->run();

    EXPECT_TRUE(messageArrived);
    EXPECT_FALSE(message.empty());
}
