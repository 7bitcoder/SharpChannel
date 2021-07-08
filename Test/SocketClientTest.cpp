#include <iostream>
#include <thread>
#include <gtest/gtest.h>
#include "SharpChannel.hpp"
#include "ChannelEventLoop.hpp"
#include "Config.hpp"

namespace
{

#if defined(SYSTEM_WINDOWS)
    std::string python = "python";
#elif defined(SYSTEM_LINUX)
    std::string python = "python3";
#endif

    std::string tester = "socketClientTester.py";
    enum TestScenario
    {
        SimplePong = 1,
        MessageReceive
    };

    std::thread runPythonClient(TestScenario testScenairo, int port)
    {
        return std::thread([testScenairo, port]() {
            std::string cmd = python + " " + tester + " " + std::to_string(testScenairo) + " " + std::to_string(port);
            std::system(cmd.c_str());
        });
    }
    class Example : public cm::IMessageObserver
    {
    private:
        bool completed = false;
        bool error = false;
        bool connected = false;
        bool sended = false;
        int received = 0;

    public:
        std::shared_ptr<cm::IChannel> comunicator;
        bool wasCompleted() { return completed; }
        bool wasError() { return error; }
        bool wasMessage() { return received > 0; }
        bool wasConnected() { return connected; }
        bool wasSended() { return sended; }
        int messages() { return received; }

        void onCompleted() override
        {
            completed = true;
        }

        void onError(const std::exception &e) override
        {
            error = true;
        }

        void onMessageReceived(const std::string &msg) override
        {
            received++;
        }

        void onConnected()
        {
            connected = true;
            sended = comunicator->sendMessage("Hello, world");
        }

        virtual ~Example() {}
    };
}

TEST(SocketClient, Subscription)
{
    cm::SocketClientSettings settings;
    settings.port = 64123;
    auto comunicator = cm::SharpChannel::makeSocketClient(settings);

    auto th = runPythonClient(TestScenario::SimplePong, settings.port);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    bool gotMessage = false, completed = false, connected = false, sended = false;

    auto onMessageReceived = [&comunicator, &gotMessage](const std::string &msg) {
        EXPECT_STRCASEEQ("Hello, world", msg.c_str());
        gotMessage = true;
    };

    auto onComplete = [&completed]() {
        completed = true;
    };

    auto onConnected = [&connected, &sended, &comunicator]() {
        connected = true;
        sended = comunicator->sendMessage("Hello, world");
    };

    comunicator->subscribe(onMessageReceived, onConnected, onComplete);

    std::thread comth([&comunicator]() {
        try
        {
            comunicator->run();
        }
        catch (...)
        {
        }
    });

    th.join();
    comth.join();

    EXPECT_TRUE(gotMessage);
    EXPECT_TRUE(completed);
    EXPECT_TRUE(connected);
    EXPECT_TRUE(sended);
}

/*TEST(SocketClient, Unsubscription)
{
    cm::SocketClientSettings settings;
    settings.port = 64124;
    auto comunicator = cm::SharpChannel::makeSocketClient(settings);
    auto th = runPythonClient(TestScenario::MessageReceive, settings.port);

    bool gotMessage = false, completed = false, connected = false, sended = false;
    auto onMessageReceived = [&gotMessage](const std::string &msg) {
        gotMessage = true;
    };

    auto onConnected = [&connected, &sended, &comunicator]() {
        connected = true;
        sended = comunicator->sendMessage("Hello, world");
    };

    auto ubsubscriber = comunicator->subscribe(onMessageReceived, onConnected);
    ubsubscriber->unsubscribe();
    std::thread comth([&comunicator]() {
        try
        {
            comunicator->run();
        }
        catch (...)
        {
        }
    });
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    bool res = comunicator->sendMessage("Hello, world");
    
    comth.join();
    th.join();

    EXPECT_FALSE(gotMessage);
    EXPECT_FALSE(completed);
    EXPECT_FALSE(connected);
    EXPECT_FALSE(sended);
}

TEST(SocketClient, NormalFinish)
{
    cm::SocketClientSettings settings;
    settings.port = 64125;
    auto comunicator = cm::SharpChannel::makeSocketClient(settings);
    auto th = runPythonClient(TestScenario::MessageReceive, settings.port);

    bool gotMessage = false, completed = false, connected = false, sended = false;
    auto onComplete = [&completed]() {
        completed = true;
    };

    comunicator->subscribe(onComplete);

    std::thread comth([&comunicator]() {
        EXPECT_ANY_THROW(comunicator->run());
    });

    comunicator->finish();

    comth.join();
    th.join();
}*/

TEST(SocketClient, Error)
{
    cm::SocketClientSettings settings;
    settings.port = 64126;

    auto comunicator = cm::SharpChannel::makeSocketClient(settings);

    bool gotMessage = false, completed = false, connected = false, sended = false, gotError = false;

    auto onMessageReceived = [&gotMessage](const std::string &msg) {
        gotMessage = true;
        throw std::runtime_error("myMessage");
    };

    auto onComplete = [&completed]() {
        completed = true;
    };    
    
    auto onConnected = [&connected, &sended, &comunicator]() {
        connected = true;
        sended = comunicator->sendMessage("Hello, world");
    };

    auto onError = [&gotError](const std::exception &error) {
        gotError = true;
    };

    auto ubsubscriber = comunicator->subscribe(onMessageReceived, onConnected, onComplete, onError);

    EXPECT_ANY_THROW(comunicator->run());

    
    EXPECT_FALSE(gotMessage);
    EXPECT_FALSE(completed);
    EXPECT_FALSE(connected);
    EXPECT_TRUE(gotError);
    EXPECT_FALSE(sended);
}

TEST(SocketClient, Parralel)
{
    cm::ChannelEventLoop loop;
    cm::SocketClientSettings settings;
    settings.port = 64127;
    std::shared_ptr<cm::IChannel> comunicator = cm::SharpChannel::makeSocketClient(settings, &loop);

    bool gotMessage = false, completed = false, connected = false, sended = false, gotError = false;

    auto th = runPythonClient(TestScenario::MessageReceive, settings.port);

    auto onMessageReceived = [](const std::string &msg) {
        EXPECT_TRUE(!msg.empty());
    };

    auto onConnected = [&connected, &sended, &comunicator]() {
        connected = true;
        sended = comunicator->sendMessage("Hello, world");
    };

    auto onCompleted = []() {
    };

    comunicator->subscribe(onMessageReceived, onConnected, onCompleted);

    std::thread thread([comunicator]() {
        comunicator->run();
    });

    int loops = 0;
    while (loop.WaitForEventAndRun())
    {
        loops++;
    }
    thread.join();
    th.join();

    EXPECT_EQ(loops, 3);
    EXPECT_TRUE(connected);
    EXPECT_TRUE(sended);
}

/*TEST(SocketClient, ParralelFullDuplex)
{
    cm::ChannelEventLoop loop;
    cm::SocketClientSettings settings;
    settings.port = 64135;
    std::shared_ptr<cm::IChannel> comunicator = cm::SharpChannel::makeSocketClient(settings, &loop);
    bool gotMessage = false, completed = false, connected = false, sended = false, gotError = false;

    auto th = runPythonClient(TestScenario::SimplePong, settings.port);

    auto onMessageReceived = [comunicator](const std::string &msg) {
        EXPECT_TRUE(!msg.empty());
    };

    auto onConnected = [&connected, &sended, &comunicator]() {
        connected = true;
        sended = comunicator->sendMessage("Hello, world");
    };

    auto onCompleted = []() {
    };

    comunicator->subscribe(onMessageReceived, onCompleted);

    std::thread thread([comunicator]() {
        comunicator->run();
    });

    int loops = 0;
    while (loop.WaitForEventAndRun())
    {
        loops++;
    }

    thread.join();
    th.join();

    EXPECT_TRUE(connected);
    EXPECT_TRUE(sended);
    EXPECT_EQ(loops, 3);
}*/

TEST(SocketClient, SubscriptionClass)
{
    cm::SocketClientSettings settings;
    settings.port = 64128;

    std::shared_ptr<cm::IChannel> comunicator = cm::SharpChannel::makeSocketClient(settings);

    auto th = runPythonClient(TestScenario::MessageReceive, settings.port);

    Example ex;
    ex.comunicator = comunicator;

    comunicator->subscribe(ex);

    std::thread thread([comunicator]() {
        comunicator->run();
    });

    th.join();
    thread.join();

    EXPECT_TRUE(ex.wasCompleted());
    EXPECT_TRUE(ex.wasSended());
    EXPECT_TRUE(ex.wasMessage());
    EXPECT_FALSE(ex.wasError());
}

/*TEST(SocketClient, UnsubscriptionClass)
{
    cm::SocketClientSettings settings;
    settings.port = 64129;
    std::shared_ptr<cm::IChannel> comunicator = cm::SharpChannel::makeSocketClient(settings);

    auto th = runPythonClient(TestScenario::MessageReceive, settings.port);

    Example ex;
    ex.comunicator = comunicator;
    auto ubsubscriber = comunicator->subscribe(ex);

    ubsubscriber->unsubscribe();
    std::thread thread([comunicator]() {
        comunicator->run();
    });
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    bool res = comunicator->sendMessage("Hello, world");

    th.join();
    thread.join();

    EXPECT_FALSE(ex.wasError());
    EXPECT_FALSE(ex.wasCompleted());
    EXPECT_FALSE(ex.wasSended());
    EXPECT_FALSE(ex.wasMessage());
}*/

TEST(SocketClient, ErrorClass)
{
    cm::SocketClientSettings settings;
    settings.port = 64130;
    std::shared_ptr<cm::IChannel> comunicator = cm::SharpChannel::makeSocketClient(settings);

    auto th = runPythonClient(TestScenario::MessageReceive, settings.port);

    class ExampleThrow : public Example
    {
    public:
        void onMessageReceived(const std::string &msg) override
        {
            Example::onMessageReceived(msg);
            throw std::runtime_error("myMessage");
        }
        void onError(const std::exception &error) override
        {
            Example::onError(error);
            EXPECT_STRCASEEQ(error.what(), "myMessage");
        }
    } ex;
    ex.comunicator = comunicator;

    auto ubsubscriber = comunicator->subscribe(ex);

    std::thread thread([comunicator]() {
        EXPECT_ANY_THROW(comunicator->run());
    });

    th.join();
    thread.join();

    EXPECT_FALSE(ex.wasCompleted());
    EXPECT_TRUE(ex.wasMessage());
    EXPECT_TRUE(ex.wasError());
    EXPECT_TRUE(ex.wasSended());
    EXPECT_EQ(ex.messages(), 1);
}

TEST(SocketClient, ParralelClass)
{
    cm::ChannelEventLoop loop;
    cm::SocketClientSettings settings;
    settings.port = 64131;
    std::shared_ptr<cm::IChannel> comunicator = cm::SharpChannel::makeSocketClient(settings, &loop);

    auto th = runPythonClient(TestScenario::MessageReceive, settings.port);

    Example ex;
    ex.comunicator = comunicator;

    comunicator->subscribe(ex);

    std::thread thread([comunicator]() {
        comunicator->run();
    });

    int loops = 0;
    while (loop.WaitForEventAndRun())
    {
        loops++;
    }
    thread.join();
    th.join();
    EXPECT_EQ(loops, 3);
    EXPECT_TRUE(ex.wasCompleted());
    EXPECT_TRUE(ex.wasMessage());
    EXPECT_TRUE(ex.wasSended());
    EXPECT_FALSE(ex.wasError());
    EXPECT_EQ(ex.messages(), 1);
}