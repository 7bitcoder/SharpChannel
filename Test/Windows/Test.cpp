#include <iostream>
#include <thread>
#include <gtest/gtest.h>
#include "SharpChannel.hpp"

TEST (RunCommand, WithCapture) { 
    cm::RunCommandSettings settings;
    settings.captureOutput = true;
    settings.command = "dir";
    auto comunicator = cm::SharpChannel::makeSystemCommand(settings);

    auto onMessageReceived = [] (const std::string& msg) {
        EXPECT_TRUE(!msg.empty());
        return cm::Control::Ok;
    };

    comunicator->subscribe(onMessageReceived);
    comunicator->run();
}

TEST (RunCommand, WithoutCapture) { 
    cm::RunCommandSettings settings;
    settings.captureOutput = false;
    settings.command = "dir";
    auto comunicator = cm::SharpChannel::makeSystemCommand(settings);

    auto onMessageReceived = [] (const std::string& msg) {
        EXPECT_EQ(msg, "0");
        return cm::Control::Ok;
    };

    comunicator->subscribe(onMessageReceived);
    comunicator->run();
}