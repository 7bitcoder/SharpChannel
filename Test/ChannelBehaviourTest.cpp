#include <iostream>
#include <thread>
#include <gtest/gtest.h>
#include "SharpChannel.hpp"
#include "ChannelEventLoop.hpp"

namespace {
    class Example: public cm::IMessageObserver {
        private:
            bool completed = false;
            bool error = false;
            bool connected = false;
            int received = 0;
        public:
            bool wasCompleted() { return completed; }
            bool wasError() { return error; }
            bool wasMessage() { return received > 0; }
            bool wasConnected() { return connected; }
            int messages() { return received; }

            void onCompleted() override {
                completed = true;
            }

            void onError(const std::exception& e) override {
                error = true;
            }

            void onMessageReceived(const std::string& msg) override{
                received++;
            }

            void onConnected() {
                connected = true;
            }

            virtual ~Example() {}
    };
}

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

    bool onMessageReceiveSw = false, onCompleteSw = false,  onErrorSw = false, onConnectedSw = false;
    
    auto onMessageReceived = [&onMessageReceiveSw](const std::string &msg) {
        onMessageReceiveSw = true;
        throw std::runtime_error("myMessage");
    };

    auto onComplete = [&onCompleteSw]() {
        onCompleteSw = true;
    };

    auto onConnected = [&onConnectedSw]() {
        onConnectedSw = true;
    };

    auto onError = [&onErrorSw](const std::exception &error) {
        onErrorSw = true;
        EXPECT_STRCASEEQ(error.what(), "myMessage");
    };

    auto ubsubscriber = comunicator->subscribe(onMessageReceived, onConnected, onComplete, onError);

    EXPECT_ANY_THROW(comunicator->run());
    
    EXPECT_TRUE(onMessageReceiveSw);
    EXPECT_FALSE(onCompleteSw);
    EXPECT_TRUE(onErrorSw);
    EXPECT_TRUE(onConnectedSw);
}

TEST(ChannelBehaviour, Parralel)
{
    cm::RunCommandSettings settings;
    settings.command = " ";
    cm::ChannelEventLoop loop;
    std::shared_ptr<cm::IReadOnlyChannel> comunicator = cm::SharpChannel::makeSystemCommand(settings, &loop);

    auto onMessageReceived = [](const std::string &msg) {
        EXPECT_TRUE(!msg.empty());
    };

    auto onConnected = [](){
    };

    comunicator->subscribe(onMessageReceived, onConnected);

    std::thread thread([comunicator](){
        comunicator->run();
    });

    int loops = 0;
    while (loop.WaitForEventAndRun())
    {
        loops++;
    }
    thread.join();
    EXPECT_EQ(loops, 3);
}

TEST(ChannelBehaviour, SubscriptionClass)
{
    cm::RunCommandSettings settings;
    settings.command = " ";
    auto comunicator = cm::SharpChannel::makeSystemCommand(settings);

    Example ex;
    comunicator->subscribe(ex);
    comunicator->run();

    EXPECT_TRUE(ex.wasCompleted());
    EXPECT_TRUE(ex.wasMessage());
    EXPECT_TRUE(ex.wasConnected());
    EXPECT_FALSE(ex.wasError());
}

TEST(ChannelBehaviour, UnsubscriptionClass)
{
    cm::RunCommandSettings settings;
    settings.command = " ";
    auto comunicator = cm::SharpChannel::makeSystemCommand(settings);

    Example ex;
    auto ubsubscriber = comunicator->subscribe(ex);

    ubsubscriber->unsubscribe();
    comunicator->run();
    
    EXPECT_FALSE(ex.wasError());
    EXPECT_FALSE(ex.wasConnected());
    EXPECT_FALSE(ex.wasCompleted());
    EXPECT_FALSE(ex.wasMessage());
}

TEST(ChannelBehaviour, ErrorClass)
{
    cm::RunCommandSettings settings;
    settings.command = " ";
    auto comunicator = cm::SharpChannel::makeSystemCommand(settings);

    class ExampleThrow: public Example {
        public:
            void onMessageReceived(const std::string& msg) override {
                Example::onMessageReceived(msg);
                throw std::runtime_error("myMessage");
            }
            void onError(const std::exception &error) override {
                Example::onError(error);
                EXPECT_STRCASEEQ(error.what(), "myMessage");
            }
    } ex;
    
    auto ubsubscriber = comunicator->subscribe(ex);

    EXPECT_ANY_THROW(comunicator->run());
    
    EXPECT_FALSE(ex.wasCompleted());
    EXPECT_TRUE(ex.wasConnected());
    EXPECT_TRUE(ex.wasMessage());
    EXPECT_TRUE(ex.wasError());
    EXPECT_EQ(ex.messages(), 1);
}

TEST(ChannelBehaviour, ParralelClass)
{
    cm::RunCommandSettings settings;
    settings.command = " ";
    cm::ChannelEventLoop loop;
    std::shared_ptr<cm::IReadOnlyChannel> comunicator = cm::SharpChannel::makeSystemCommand(settings, &loop);
    
    Example ex;

    comunicator->subscribe(ex);

    std::thread thread([comunicator](){
        comunicator->run();
    });

    int loops = 0;
    while (loop.WaitForEventAndRun())
    {
        loops++;
    }
    thread.join();
    EXPECT_EQ(loops, 3);
    EXPECT_TRUE(ex.wasCompleted());
    EXPECT_TRUE(ex.wasConnected());
    EXPECT_TRUE(ex.wasMessage());
    EXPECT_FALSE(ex.wasError());
    EXPECT_EQ(ex.messages(), 1);
}
