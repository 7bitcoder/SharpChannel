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

    bool sw = false;
    auto onMessageReceived = [&sw](const std::string &msg) {
        sw = true;
    };

    auto ubsubscriber = comunicator->subscribe(onMessageReceived);
    ubsubscriber->unsubscribe();
    comunicator->run();
    
    EXPECT_FALSE(sw);
}

TEST(ChannelBehaviour, Error)
{
    cm::RunCommandSettings settings;
    settings.command = " ";
    auto comunicator = cm::SharpChannel::makeSystemCommand(settings);

    bool onMessageReceiveSw = false, onCompleteSw = false,  onErrorSw = false;
    
    auto onMessageReceived = [&onMessageReceiveSw](const std::string &msg) {
        onMessageReceiveSw = true;
        throw std::runtime_error("myMessage");
    };

    auto onComplete = [&onCompleteSw]() {
        onCompleteSw = true;
    };

    auto onError = [&onErrorSw](const std::exception &error) {
        onErrorSw = true;
        EXPECT_STRCASEEQ(error.what(), "myMessage");
    };

    auto ubsubscriber = comunicator->subscribe(onMessageReceived, onComplete, onError);

    EXPECT_ANY_THROW(comunicator->run());
    
    EXPECT_TRUE(onMessageReceiveSw);
    EXPECT_FALSE(onCompleteSw);
    EXPECT_TRUE(onErrorSw);
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