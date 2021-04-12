#include <iostream>
#include <thread>
#include <gtest/gtest.h>
#include "SharpChannel.hpp"
#include "ChannelEventLoop.hpp"
#include "Config.hpp"

#if defined(SYSTEM_WINDOWS)
    std::string python = "python";
#elif defined(SYSTEM_LINUX)
    std::string python = "python3";
#endif

std::string tester = "socketServerTester.py";

enum TestScenario {
    SimplePong = 1,
    MessageReceive
};

std::thread runPythonClient(TestScenario testScenairo, int port) {
    return std::thread([testScenairo, port](){
        std::string cmd = python +  " " + tester + " "  + std::to_string(testScenairo) + " " + std::to_string(port);
        std::system(cmd.c_str());
    });
}
class Example: public cm::IMessageObserver {
    private:
        bool completed = false;
        bool error = false;
        int received = 0;
    public:
        bool wasCompleted() { return completed; }
        bool wasError() { return error; }
        bool wasMessage() { return received > 0; }
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

        virtual ~Example() {}
};


TEST(SocketServer, Subscription)
{
    cm::SocketServerSettings settings;
    settings.port = 65123;
    auto comunicator = cm::SharpChannel::makeSocketServer(settings);

    auto th = runPythonClient(TestScenario::SimplePong, settings.port);
    
    bool gotMessage = false, completed = false;
    
    auto onMessageReceived = [&comunicator, &gotMessage](const std::string &msg) {
        EXPECT_STRCASEEQ("Hello, world", msg.c_str());
        gotMessage = true;
        comunicator->sendMessage("finish");
    };

    auto onComplete = [&completed](){
        completed = true;
    };

    comunicator->subscribe(onMessageReceived, onComplete);
    comunicator->run();
    
    
    EXPECT_TRUE(gotMessage);
    EXPECT_TRUE(completed);
    th.join();
}

TEST(SocketServer, Unsubscription)
{
    cm::SocketServerSettings settings;
    settings.port = 65124;
    auto comunicator = cm::SharpChannel::makeSocketServer(settings);
    auto th = runPythonClient(TestScenario::MessageReceive, settings.port);

    bool sw = false;
    auto onMessageReceived = [&sw](const std::string &msg) {
        sw = true;
    };

    auto ubsubscriber = comunicator->subscribe(onMessageReceived);
    ubsubscriber->unsubscribe();
    comunicator->run();
    
    EXPECT_FALSE(sw);
    th.join();
}

TEST(SocketServer, NormalFinish)
{
    cm::SocketServerSettings settings;
    settings.port = 65125;
    auto comunicator = cm::SharpChannel::makeSocketServer(settings);

    std::thread th([&comunicator](){
        bool gotMessage = false, completed = false;
        auto onComplete = [&completed](){
        completed = true;
        };

        comunicator->subscribe(onComplete);
        try {
            comunicator->run();
        } catch(...) {}
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    comunicator->finish();

    th.join();
}

TEST(SocketServer, Error)
{
    cm::SocketServerSettings settings;
    settings.port = 65126;

    auto th = runPythonClient(TestScenario::MessageReceive, settings.port);

    auto comunicator = cm::SharpChannel::makeSocketServer(settings);
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

    th.join();
}

TEST(SocketServer, Parralel)
{
    cm::ChannelEventLoop loop;
    cm::SocketServerSettings settings;
    settings.port = 65127;
    std::shared_ptr<cm::IChannel> comunicator = cm::SharpChannel::makeSocketServer(settings, &loop);
    
    auto th = runPythonClient(TestScenario::MessageReceive, settings.port);
    
    auto onMessageReceived = [](const std::string &msg) {
        EXPECT_TRUE(!msg.empty());
    };

    auto onCompleted = [](){
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
    th.join();
    
    EXPECT_EQ(loops, 2);
}

TEST(SocketServer, ParralelFullDuplex)
{
    cm::ChannelEventLoop loop;
    cm::SocketServerSettings settings;
    settings.port = 65135;
    std::shared_ptr<cm::IChannel> comunicator = cm::SharpChannel::makeSocketServer(settings, &loop);
    
    auto th = runPythonClient(TestScenario::SimplePong, settings.port);
    
    auto onMessageReceived = [comunicator](const std::string &msg) {
        EXPECT_TRUE(!msg.empty());
        comunicator->sendMessage("sendExample");
    };

    auto onCompleted = [](){
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
    th.join();
    
    EXPECT_EQ(loops, 2);
}

TEST(SocketServer, SubscriptionClass)
{
    cm::SocketServerSettings settings;
    settings.port = 65128;
    auto comunicator = cm::SharpChannel::makeSocketServer(settings);
    
    auto th = runPythonClient(TestScenario::MessageReceive, settings.port);

    Example ex;
    comunicator->subscribe(ex);
    comunicator->run();
    
    th.join();

    EXPECT_TRUE(ex.wasCompleted());
    EXPECT_TRUE(ex.wasMessage());
    EXPECT_FALSE(ex.wasError());
}

TEST(SocketServer, UnsubscriptionClass)
{
    cm::SocketServerSettings settings;
    settings.port = 65129;
    auto comunicator = cm::SharpChannel::makeSocketServer(settings);

    auto th = runPythonClient(TestScenario::MessageReceive, settings.port);

    Example ex;
    auto ubsubscriber = comunicator->subscribe(ex);

    ubsubscriber->unsubscribe();
    comunicator->run();
    
    th.join();

    EXPECT_FALSE(ex.wasError());
    EXPECT_FALSE(ex.wasCompleted());
    EXPECT_FALSE(ex.wasMessage());
}

TEST(SocketServer, ErrorClass)
{
    cm::SocketServerSettings settings;
    settings.port = 65130;
    auto comunicator = cm::SharpChannel::makeSocketServer(settings);

    auto th = runPythonClient(TestScenario::MessageReceive, settings.port);

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
    
    th.join();
    
    EXPECT_FALSE(ex.wasCompleted());
    EXPECT_TRUE(ex.wasMessage());
    EXPECT_TRUE(ex.wasError());
    EXPECT_EQ(ex.messages(), 1);
}

TEST(SocketServer, ParralelClass)
{
    cm::ChannelEventLoop loop;
    cm::SocketServerSettings settings;
    settings.port = 65131;
    std::shared_ptr<cm::IChannel> comunicator = cm::SharpChannel::makeSocketServer(settings, &loop);
    
    auto th = runPythonClient(TestScenario::MessageReceive, settings.port);
    
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
    th.join();
    EXPECT_EQ(loops, 2);
    EXPECT_TRUE(ex.wasCompleted());
    EXPECT_TRUE(ex.wasMessage());
    EXPECT_FALSE(ex.wasError());
    EXPECT_EQ(ex.messages(), 1);
}

TEST(SocketServer, ParralelFullDuplexClass)
{
    cm::ChannelEventLoop loop;
    cm::SocketServerSettings settings;
    settings.port = 65137;
    std::shared_ptr<cm::IChannel> comunicator = cm::SharpChannel::makeSocketServer(settings, &loop);
    
    auto th = runPythonClient(TestScenario::SimplePong, settings.port);
    
    class ExampleFullDuplex: public Example {
        public:
            std::shared_ptr<cm::IChannel> comunicator;
            void onMessageReceived(const std::string& msg) override {
                Example::onMessageReceived(msg);
                comunicator->sendMessage("sendExample");
            }
            void onError(const std::exception &error) override {
                Example::onError(error);
                EXPECT_STRCASEEQ(error.what(), "myMessage");
            }
    } ex;
    ex.comunicator = comunicator;

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
    th.join();

    EXPECT_TRUE(ex.wasCompleted());
    EXPECT_TRUE(ex.wasMessage());
    EXPECT_FALSE(ex.wasError());
    EXPECT_EQ(ex.messages(), 1);
    EXPECT_EQ(loops, 2);
}