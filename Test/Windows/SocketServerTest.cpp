#include <iostream>
#include <thread>
#include <gtest/gtest.h>
#include "SharpChannel.hpp"

TEST(SocketServer, Normal)
{
    cm::SocketServerSettings settings;
    settings.port = 65123;
    auto comunicator = cm::SharpChannel::makeSocketServer(settings);
    auto port = settings.port;
    std::thread runPythonClient([port](){
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        cm::RunCommandSettings settings;
        settings.command = std::string("python socketServerTester.py 1 ") + std::to_string(port);
        auto comunicator = cm::SharpChannel::makeSystemCommand(settings);
        comunicator->run();
    });


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
    runPythonClient.join();
}

TEST(SocketServer, NormalFinish)
{
    cm::SocketServerSettings settings;
    settings.port = 65123;
    auto comunicator = cm::SharpChannel::makeSocketServer(settings);
    


    std::thread runPythonClient([&comunicator](){
        bool gotMessage = false, completed = false;
        auto onComplete = [&completed](){
        completed = true;
        };

        comunicator->subscribe(onComplete);
        comunicator->run();
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    comunicator->finish();

    runPythonClient.join();
}
