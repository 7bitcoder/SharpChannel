#include <iostream>
#include <thread>
#include <gtest/gtest.h>
#include "SharpChannel.hpp"
#include "ChannelEventLoop.hpp"

TEST(ChannelBehaviour, Subscription)
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

TEST(ChannelBehaviour, Unsubscription)
{
    cm::RunCommandSettings settings;
    settings.command = " ";
    auto comunicator = cm::SharpChannel::makeSystemCommand(settings);

    auto onMessageReceived = [](const std::string &msg) {
        throw std::exception();
    };

    auto ubsubscriber = comunicator->subscribe(onMessageReceived);
    ubsubscriber->unsubscribe();

    EXPECT_NO_THROW(comunicator->run());
}

TEST(ChannelBehaviour, Parralel)
{
    cm::RunCommandSettings settings;
    settings.command = " ";
    cm::ChannelEventLoop loop;
    std::shared_ptr<cm::IReadOnlyChannel> comunicator = cm::SharpChannel::makeSystemCommand(settings, &loop);
    bool finish = false;

    auto onMessageReceived = [](const std::string &msg) {
        EXPECT_TRUE(!msg.empty());
    };

    auto onCompleted = [&finish](){
        finish = true;
    };

    comunicator->subscribe(onMessageReceived, onCompleted);

    std::thread thread([comunicator](){
        comunicator->run();
    });

    int loops = 0;
    while (loop.WaitForEventAndRun())
    {
        loops++;
    }
    thread.join();
    EXPECT_EQ(loops, 2);
}