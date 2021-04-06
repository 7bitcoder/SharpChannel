#include <iostream>
#include <thread>
#include "SharpChannel.hpp"

class Handler: public cm::IMessageObserver {
    private:
        std::unique_ptr<cm::IChannel> _comunicator; 
    public:
        Handler() {
            cm::SocketServerSettings settings;

            _comunicator = cm::SharpChannel::makeSocketServer(settings);
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
            
            _comunicator->sendMessage(messageReceived);
            return cm::Control::Ok;
        }

        cm::Control onMessageReceived(const std::string& messageReceived) override { return this->operator()(messageReceived);}
        void onCompleted() override { std::cout << "complete: " << std::endl; }
        ~Handler() {}
};

int main(int, char**) {

    cm::SocketServerSettings settings;
    auto comunicator = cm::SharpChannel::makeSocketServer(settings);

    auto onMessageReceived = [&comunicator] (const std::string& msg) {
        std::cout << "message received: " << msg << std::endl;
        comunicator->sendMessage(msg);
        return cm::Control::Ok;
    };

    auto onDataReceived = [&comunicator] (const char* data, size_t lenght) {
        std::cout << "message Raw received: " << data << std::endl;
        comunicator->sendData(data, lenght);
        return cm::Control::Ok;
    };


    comunicator->subscribe(onMessageReceived);
    comunicator->subscribe(onDataReceived);
    
    std::thread thread_object([&comunicator]() {
        std::cout << "in thread";
        int cnt = 0;
        while (cnt < 20) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::string msg("cnt :");
            msg += std::to_string(cnt++);
            comunicator->sendMessage(msg);
        }
    });
    comunicator->run();
    thread_object.join();
}
