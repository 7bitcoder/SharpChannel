#include <iostream>
#include <thread>
#include <gtest/gtest.h>
#include "SharpChannel.hpp"

namespace
{
    class Example : public cm::IChannelMessageObserver
    {
    private:
        bool completed = false;
        bool err = false;
        int received = 0;

    public:
        bool wasCompleted() { return completed; }
        bool wasError() { return err; }
        bool wasMessage() { return received > 0; }
        int messages() { return received; }

        void error(const std::exception &e) override
        {
            err = true;
        }

        void complete() override
        {
            completed = true;
        }

        bool next(const std::string &message) override
        {
            received++;
            return true;
        }

        ~Example() {}
    };
}

TEST(ChannelBehaviour, Subscription)
{
    cm::RunCommandSettings settings;
    settings.command = " ";
    auto comunicator = cm::CommandChannel::create(settings);

    auto onMessageReceived = [](const std::string &msg)
    {
        EXPECT_TRUE(!msg.empty());
    };

    comunicator->subscribe(onMessageReceived);
    comunicator->run();
}

TEST(ChannelBehaviour, Unsubscription)
{
    cm::RunCommandSettings settings;
    settings.command = " ";
    auto comunicator = cm::CommandChannel::create(settings);

    bool sw = false;
    auto onMessageReceived = [&sw](const std::string &msg)
    {
        sw = true;
    };

    auto ubsubscriber = comunicator->subscribe(onMessageReceived);
    ubsubscriber->unsubscribe();
    comunicator->run();

    EXPECT_FALSE(sw);
}

TEST(ChannelBehaviour, UnsubscriptionSuccesful)
{
    cm::RunCommandSettings settings;
    settings.command = " ";
    auto comunicator = cm::CommandChannel::create(settings);

    auto onMessageReceived = [](const std::string &msg) {
    };

    auto ubsubscriber = comunicator->subscribe(onMessageReceived);
    EXPECT_TRUE(ubsubscriber->unsubscribe());
}

TEST(ChannelBehaviour, UnsubscriptionPack)
{
    cm::RunCommandSettings settings;
    settings.command = " ";
    auto comunicator = cm::CommandChannel::create(settings);

    bool sw = false;
    auto onMessageReceived = [&sw](const std::string &msg)
    {
        sw = true;
    };

    auto onComplete = [&sw]()
    {
        sw = true;
    };

    auto onError = [&sw](const std::exception &e)
    {
        sw = true;
    };

    auto ubsubscriber = comunicator->subscribe(onMessageReceived, onComplete, onError);
    ubsubscriber->unsubscribe();
    comunicator->run();

    EXPECT_FALSE(sw);
}

TEST(ChannelBehaviour, UnsubscriptionPackSuccesful)
{
    cm::RunCommandSettings settings;
    settings.command = " ";
    auto comunicator = cm::CommandChannel::create(settings);

    auto onMessageReceived = [](const std::string &msg) {
    };

    auto onComplete = []() {
    };

    auto onError = [](const std::exception &e) {
    };

    auto ubsubscriber = comunicator->subscribe(onMessageReceived, onComplete, onError);
    EXPECT_TRUE(ubsubscriber->unsubscribe());
}

TEST(ChannelBehaviour, MultipleUnsubscription)
{
    cm::RunCommandSettings settings;
    settings.command = " ";
    auto comunicator = cm::CommandChannel::create(settings);

    bool sw = false;
    auto onMessageReceived1 = [&sw](const std::string &msg)
    {
        sw = true;
    };

    auto ubsubscriber1 = comunicator->subscribe(onMessageReceived1);

    auto onMessageReceived2 = [&sw](const std::string &msg)
    {
        sw = true;
    };

    auto ubsubscriber2 = comunicator->subscribe(onMessageReceived2);

    auto onComplete = [&sw]()
    {
        sw = true;
    };

    auto ubsubscriber3 = comunicator->subscribe(onComplete);

    ubsubscriber1->unsubscribe();
    ubsubscriber2->unsubscribe();
    ubsubscriber3->unsubscribe();
    comunicator->run();

    EXPECT_FALSE(sw);
}

TEST(ChannelBehaviour, OneOfManyUnsubscription)
{
    cm::RunCommandSettings settings;
    settings.command = " ";
    auto comunicator = cm::CommandChannel::create(settings);

    bool sw1 = false;
    auto onMessageReceived = [&sw1](const std::string &msg)
    {
        sw1 = true;
    };

    auto ubsubscriber1 = comunicator->subscribe(onMessageReceived);

    bool sw2 = false;
    auto onMessageReceived2 = [&sw2](const std::string &msg)
    {
        sw2 = true;
    };

    auto ubsubscriber2 = comunicator->subscribe(onMessageReceived2);

    bool sw3 = false;
    auto onComplete = [&sw3]()
    {
        sw3 = true;
    };

    auto ubsubscriber3 = comunicator->subscribe(onComplete);

    ubsubscriber2->unsubscribe();
    comunicator->run();

    EXPECT_TRUE(sw1);
    EXPECT_FALSE(sw2);
    EXPECT_TRUE(sw3);
}

TEST(ChannelBehaviour, UnsubscriptionOnDeletedObject)
{
    cm::IUnsubscribable::Ptr unsubscriber;
    {
        cm::RunCommandSettings settings;
        settings.command = " ";
        auto comunicator = cm::CommandChannel::create(settings);

        bool sw = false;
        auto onMessageReceived = [&sw](const std::string &msg)
        {
            sw = true;
        };

        unsubscriber = comunicator->subscribe(onMessageReceived);
    }

    EXPECT_FALSE(unsubscriber->unsubscribe());
}

TEST(ChannelBehaviour, Error)
{
    cm::RunCommandSettings settings;
    settings.command = " ";
    auto comunicator = cm::CommandChannel::create(settings);

    bool onMessageReceiveSw = false, onCompleteSw = false, onErrorSw = false;

    auto onMessageReceived = [&onMessageReceiveSw](const std::string &msg)
    {
        onMessageReceiveSw = true;
        throw std::runtime_error("myMessage");
    };

    auto onComplete = [&onCompleteSw]()
    {
        onCompleteSw = true;
    };

    auto onError = [&onErrorSw](const std::exception &error)
    {
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
    auto comunicator = cm::CommandChannel::create(settings, &loop);

    auto onMessageReceived = [](const std::string &msg)
    {
        EXPECT_TRUE(!msg.empty());
    };

    comunicator->subscribe(onMessageReceived);

    std::thread thread([comunicator]()
                       { comunicator->run(); });

    int loops = 0;
    while (loop.WaitForEventAndRun())
    {
        loops++;
    }
    thread.join();
    EXPECT_EQ(loops, 2);
}

TEST(ChannelBehaviour, SubscriptionClass)
{
    cm::RunCommandSettings settings;
    settings.command = " ";
    auto comunicator = cm::CommandChannel::create(settings);

    Example ex;
    comunicator->subscribe(ex);
    comunicator->run();

    EXPECT_TRUE(ex.wasCompleted());
    EXPECT_TRUE(ex.wasMessage());
    EXPECT_FALSE(ex.wasError());
}

TEST(ChannelBehaviour, UnsubscriptionClass)
{
    cm::RunCommandSettings settings;
    settings.command = " ";
    auto comunicator = cm::CommandChannel::create(settings);

    Example ex;
    auto ubsubscriber = comunicator->subscribe(ex);

    ubsubscriber->unsubscribe();
    comunicator->run();

    EXPECT_FALSE(ex.wasError());
    EXPECT_FALSE(ex.wasCompleted());
    EXPECT_FALSE(ex.wasMessage());
}

TEST(ChannelBehaviour, ErrorClass)
{
    cm::RunCommandSettings settings;
    settings.command = " ";
    auto comunicator = cm::CommandChannel::create(settings);

    class ExampleThrow : public Example
    {
    public:
        bool next(const std::string &msg) override
        {
            Example::next(msg);
            throw std::runtime_error("myMessage");
        }
        void error(const std::exception &err) override
        {
            Example::error(err);
            EXPECT_STRCASEEQ(err.what(), "myMessage");
        }
    } ex;

    auto ubsubscriber = comunicator->subscribe(ex);

    EXPECT_ANY_THROW(comunicator->run());

    EXPECT_FALSE(ex.wasCompleted());
    EXPECT_TRUE(ex.wasMessage());
    EXPECT_TRUE(ex.wasError());
    EXPECT_EQ(ex.messages(), 1);
}

TEST(ChannelBehaviour, ParralelClass)
{
    cm::RunCommandSettings settings;
    settings.command = " ";
    cm::ChannelEventLoop loop;
    auto comunicator = cm::CommandChannel::create(settings, &loop);

    Example ex;

    comunicator->subscribe(ex);

    std::thread thread([comunicator]()
                       { comunicator->run(); });

    int loops = 0;
    while (loop.WaitForEventAndRun())
    {
        loops++;
    }
    thread.join();
    EXPECT_EQ(loops, 2);
    EXPECT_TRUE(ex.wasCompleted());
    EXPECT_TRUE(ex.wasMessage());
    EXPECT_FALSE(ex.wasError());
    EXPECT_EQ(ex.messages(), 1);
}
