#include <iostream>
#include <thread>
#include <gtest/gtest.h>
#include "CommandChannel.hpp"

TEST(CommandChannelTest, Normal)
{
    cm::RunCommandSettings settings;
    settings.command = " ";
    auto comunicator = cm::CommandChannel::create(settings);

    auto onMessageReceived = [](const std::string &msg) {
        EXPECT_TRUE(!msg.empty());
    };

    comunicator->subscribe(onMessageReceived);
    comunicator->run();
}
