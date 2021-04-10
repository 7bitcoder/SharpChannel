#include <iostream>
#include <thread>
#include <gtest/gtest.h>
#include "SharpChannel.hpp"

TEST(RunCommand, Normal)
{
    cm::RunCommandSettings settings;
    settings.command = " ";
    auto comunicator = cm::SharpChannel::makeSystemCommand(settings);

    auto onMessageReceived = [](const std::string &msg) {
        EXPECT_TRUE(!msg.empty());
    };

    comunicator->subscribe(onMessageReceived);
    comunicator->run();
}
