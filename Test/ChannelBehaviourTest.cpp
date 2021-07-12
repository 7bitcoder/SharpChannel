#include <iostream>
#include <thread>
#include <gtest/gtest.h>
#include "SharpChannel.hpp"

class ChannelBehaviourTest : public ::testing::Test
{
protected:
    ChannelBehaviourTest() {}

    void SetUp() override
    {
        cm::RunCommandSettings settings;
        settings.command = " ";
        comunicator = cm::CommandChannel::create(settings);
    }

    void TearDown() override {}

    ~ChannelBehaviourTest() {}

    static void TearDownTestSuite() {}

    cm::CommandChannel::Ptr comunicator;
};

class Example : public cm::IChannelMessageObserver
{
private:
    bool completed = false;
    bool err = false;
    int received = 0;

public:
    bool wasCompleted() { return completed; }
    bool errorOccured() { return err; }
    bool messageArrived() { return received > 0; }
    int messages() { return received; }

    void error(const std::exception &e) override { err = true; }
    void complete() override { completed = true; }
    bool next(const std::string &message) override
    {
        received++;
        return true;
    }

    ~Example() {}
};

class ExampleThrow : public Example
{
public:
    std::string &getErrorMessage() { return errorMsg; }
    bool next(const std::string &msg) override
    {
        Example::next(msg);
        throw std::runtime_error("myMessage");
    }
    void error(const std::exception &err) override
    {
        Example::error(err);
        errorMsg = err.what();
    }

private:
    std::string errorMsg;
};

TEST_F(ChannelBehaviourTest, SubscriptionSignleCallback)
{
    bool messageArrived = false;
    std::string message;

    auto onMessageReceived = [&messageArrived, &message](const std::string &msg)
    {
        message = msg;
        messageArrived = true;
    };

    comunicator->subscribe(onMessageReceived);
    comunicator->run();

    EXPECT_TRUE(messageArrived);
    EXPECT_FALSE(message.empty());
}

TEST_F(ChannelBehaviourTest, SubscriptionManySignaleCallbacks)
{
    bool messageArrived = false, wasCompleted = false, errorOccured = false;
    std::string message;

    auto onMessageReceived = [&messageArrived, &message](const std::string &msg)
    {
        message = msg;
        messageArrived = true;
    };

    auto onComplete = [&wasCompleted]()
    {
        wasCompleted = true;
    };

    auto onError = [&errorOccured](const std::exception &error)
    {
        errorOccured = true;
    };

    comunicator->subscribe(onMessageReceived);
    comunicator->subscribe(onComplete);
    comunicator->subscribe(onError);
    comunicator->run();

    EXPECT_TRUE(messageArrived);
    EXPECT_FALSE(message.empty());
    EXPECT_TRUE(wasCompleted);
    EXPECT_FALSE(errorOccured);
}

TEST_F(ChannelBehaviourTest, SubscriptionPackCallbacks)
{
    bool messageArrived = false, wasCompleted = false, errorOccured = false;
    std::string message;

    auto onMessageReceived = [&messageArrived, &message](const std::string &msg)
    {
        message = msg;
        messageArrived = true;
    };

    auto onComplete = [&wasCompleted]()
    {
        wasCompleted = true;
    };

    auto onError = [&errorOccured](const std::exception &error)
    {
        errorOccured = true;
    };

    comunicator->subscribe(onMessageReceived, onComplete, onError);
    comunicator->run();

    EXPECT_TRUE(messageArrived);
    EXPECT_FALSE(message.empty());
    EXPECT_TRUE(wasCompleted);
    EXPECT_FALSE(errorOccured);
}

TEST_F(ChannelBehaviourTest, UnsubscriptionSignleCallback)
{
    bool messageArrived = false;

    auto onMessageReceived = [&messageArrived](const std::string &msg)
    {
        messageArrived = true;
    };

    auto ubsubscriber = comunicator->subscribe(onMessageReceived);
    ubsubscriber->unsubscribe();
    comunicator->run();

    EXPECT_FALSE(messageArrived);
}

TEST_F(ChannelBehaviourTest, UnsubscriptionCheckSingleCallback)
{
    auto onMessageReceived = [](const std::string &msg) {};

    auto ubsubscriber = comunicator->subscribe(onMessageReceived);

    EXPECT_TRUE(ubsubscriber->unsubscribe());
}

TEST_F(ChannelBehaviourTest, UnsubscriptionPackCallbacks)
{
    bool messageArrived = false, wasCompleted = false, errorOccured = false;

    auto onMessageReceived = [&messageArrived](const std::string &msg)
    {
        messageArrived = true;
    };

    auto onComplete = [&wasCompleted]()
    {
        wasCompleted = true;
    };

    auto onError = [&errorOccured](const std::exception &error)
    {
        errorOccured = true;
    };

    auto ubsubscriber = comunicator->subscribe(onMessageReceived, onComplete, onError);
    ubsubscriber->unsubscribe();
    comunicator->run();

    EXPECT_FALSE(messageArrived);
    EXPECT_FALSE(wasCompleted);
    EXPECT_FALSE(errorOccured);
}

TEST_F(ChannelBehaviourTest, UnsubscriptionCheckPackCallbacks)
{
    auto onMessageReceived = [](const std::string &msg) {};
    auto onComplete = []() {};
    auto onError = [](const std::exception &e) {};

    auto ubsubscriber = comunicator->subscribe(onMessageReceived, onComplete, onError);

    EXPECT_TRUE(ubsubscriber->unsubscribe());
}

TEST_F(ChannelBehaviourTest, UnsubscriptionManySignaleCallbacks)
{
    bool messageArrived = false, wasCompleted = false, errorOccured = false;

    auto onMessageReceived = [&messageArrived](const std::string &msg)
    {
        messageArrived = true;
    };

    auto onComplete = [&wasCompleted]()
    {
        wasCompleted = true;
    };

    auto onError = [&errorOccured](const std::exception &error)
    {
        errorOccured = true;
    };

    auto ubsubscriber1 = comunicator->subscribe(onMessageReceived);
    auto ubsubscriber2 = comunicator->subscribe(onComplete);
    auto ubsubscriber3 = comunicator->subscribe(onError);

    ubsubscriber1->unsubscribe();
    ubsubscriber2->unsubscribe();
    ubsubscriber3->unsubscribe();
    comunicator->run();

    EXPECT_FALSE(messageArrived);
    EXPECT_FALSE(wasCompleted);
    EXPECT_FALSE(errorOccured);
}

TEST_F(ChannelBehaviourTest, UnsubscriptionSomeSignaleCallbacks)
{
    bool messageArrived = false, wasCompleted = false, errorOccured = false;

    auto onMessageReceived = [&messageArrived](const std::string &msg)
    {
        messageArrived = true;
    };

    auto onComplete = [&wasCompleted]()
    {
        wasCompleted = true;
    };

    auto onError = [&errorOccured](const std::exception &error)
    {
        errorOccured = true;
    };

    auto ubsubscriber1 = comunicator->subscribe(onMessageReceived);
    auto ubsubscriber2 = comunicator->subscribe(onComplete);
    auto ubsubscriber3 = comunicator->subscribe(onError);

    ubsubscriber1->unsubscribe();
    ubsubscriber3->unsubscribe();
    comunicator->run();

    EXPECT_FALSE(messageArrived);
    EXPECT_TRUE(wasCompleted);
    EXPECT_FALSE(errorOccured);
}

TEST_F(ChannelBehaviourTest, UnsubscriptionOnDeletedObject)
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

TEST_F(ChannelBehaviourTest, Error)
{
    bool messageArrived = false, wasCompleted = false, errorOccured = false;
    std::string errorMessage;

    auto onMessageReceived = [&messageArrived](const std::string &msg)
    {
        messageArrived = true;
        throw std::runtime_error("myMessage");
    };

    auto onComplete = [&wasCompleted]()
    {
        wasCompleted = true;
    };

    auto onError = [&errorOccured, &errorMessage](const std::exception &error)
    {
        errorOccured = true;
        errorMessage = error.what();
    };

    auto ubsubscriber = comunicator->subscribe(onMessageReceived, onComplete, onError);

    EXPECT_ANY_THROW(comunicator->run());

    EXPECT_STRCASEEQ(errorMessage.c_str(), "myMessage");
    EXPECT_TRUE(messageArrived);
    EXPECT_FALSE(wasCompleted);
    EXPECT_TRUE(errorOccured);
}

TEST_F(ChannelBehaviourTest, Parralel)
{
    cm::RunCommandSettings settings;
    settings.command = " ";
    cm::ChannelEventLoop loop;
    auto comunicator = cm::CommandChannel::create(settings, &loop);

    bool messageArrived = false;
    std::string message;

    auto onMessageReceived = [&messageArrived, &message](const std::string &msg)
    {
        messageArrived = true;
        message = msg;
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
    EXPECT_FALSE(message.empty());
}

TEST_F(ChannelBehaviourTest, SubscriptionClass)
{
    Example ex;
    comunicator->subscribe(ex);
    comunicator->run();

    EXPECT_TRUE(ex.wasCompleted());
    EXPECT_TRUE(ex.messageArrived());
    EXPECT_FALSE(ex.errorOccured());
}

TEST_F(ChannelBehaviourTest, UnsubscriptionClass)
{
    Example ex;
    auto ubsubscriber = comunicator->subscribe(ex);

    ubsubscriber->unsubscribe();
    comunicator->run();

    EXPECT_FALSE(ex.errorOccured());
    EXPECT_FALSE(ex.wasCompleted());
    EXPECT_FALSE(ex.messageArrived());
}

TEST_F(ChannelBehaviourTest, UnsubscriptionClassCheck)
{
    Example ex;
    auto ubsubscriber = comunicator->subscribe(ex);

    EXPECT_TRUE(ubsubscriber->unsubscribe());
}

TEST_F(ChannelBehaviourTest, ErrorClass)
{

    class ExampleThrow ex;

    auto ubsubscriber = comunicator->subscribe(ex);

    EXPECT_ANY_THROW(comunicator->run());

    EXPECT_STRCASEEQ(ex.getErrorMessage().c_str(), "myMessage");
    EXPECT_FALSE(ex.wasCompleted());
    EXPECT_TRUE(ex.messageArrived());
    EXPECT_TRUE(ex.errorOccured());
    EXPECT_EQ(ex.messages(), 1);
}

TEST_F(ChannelBehaviourTest, ParralelClass)
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
    EXPECT_TRUE(ex.messageArrived());
    EXPECT_FALSE(ex.errorOccured());
    EXPECT_EQ(ex.messages(), 1);
}
