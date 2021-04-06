#include <iostream>
#include "ChannelManager.hpp"

class Handler: public cm::IMessageObserver {
    private:
        std::unique_ptr<cm::IChannel> _comunicator; 
    public:
        Handler() {
            cm::SocketServerSettings settings;

            _comunicator = cm::ChannelManager::makeSocketServer(settings);
        }

        void run() {
            auto unsubscriber = _comunicator->subscribe(*this);

            _comunicator->run();
            
            unsubscriber->unsunscribe();
        }

        cm::Control operator()(const std::string& messageReceived)  {
            if(messageReceived == "exit") {
                std::cout << "exiting: " << std::endl;
                return cm::Control::Stop;
            }
            std::cout << "message received: " << messageReceived << std::endl;
            
            _comunicator->send(messageReceived);
            return cm::Control::Ok;
        }

        cm::Control onMessageReceived(const std::string& messageReceived) override { return this->operator()(messageReceived);}
        void onComplete() override { std::cout << "complete: " << std::endl; }
        ~Handler() {}
};

int main(int, char**) {

    cm::SocketServerSettings settings;
    auto comunicator = cm::ChannelManager::makeSocketServer(settings);

    auto onMessageReceived = [&comunicator] (const std::string& msg) {
        std::cout << "message received: " << msg << std::endl;
        comunicator->send(msg);
        return cm::Control::Ok;
    };

    auto onRawReceive = [&comunicator] (const char* data, size_t lenght) {
        std::cout << "message Raw received: " << data << std::endl;
        comunicator->send(data, lenght);
        return cm::Control::Ok;
    };


    comunicator->subscribe(onMessageReceived);
    comunicator->subscribe(onRawReceive);
    
    std::thread thread_object([&comunicator]() {
        std::cout << "in thread";
        int cnt = 0;
        while (cnt < 20) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::string msg("cnt :");
            msg += std::to_string(cnt++);
            comunicator->send(msg);
        }
    });
    comunicator->run();
    thread_object.join();
}
