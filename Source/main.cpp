#include <iostream>
#include "ProcessManager.hpp"

class Handler {
    public:
        pm::Control operator()(const std::string& messageReceived)  {
            if(messageReceived == "exit") {
                std::cout << "exiting: " << std::endl;
                return pm::Control::Stop;
            }
            std::cout << "message received: " << messageReceived << std::endl;
            return pm::Control::Ok;
        }
        ~Handler() {}
};

int main(int, char**) {
    Handler g;
    std::function<void(int)> f;
    //std::shared_ptr<pm::MessageHandler> h = std::make_shared<Handler>();
    auto comunicator = pm::ProcessManager::makeSTDComunicator("./python/python.exe main.py");
    //manager.useSTD().bind();
    auto unsubscriber = comunicator->subscribe(g);
    auto unsubscriber2 = comunicator->subscribe(g);
    auto unsubscriber3 = comunicator->subscribe(g);

    comunicator->run();
    
    unsubscriber->unsunscribe();
    unsubscriber2->unsunscribe();
    unsubscriber3->unsunscribe();

}
