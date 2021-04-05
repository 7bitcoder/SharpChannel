#include <iostream>
#include "ProcessManager.hpp"

class Handler: public pm::IObserver {
    private:
        std::unique_ptr<pm::ISubject> _comunicator; 
    public:
        Handler() {
            pm::SocketServerSettings settings;

            _comunicator = pm::ProcessManager::makeSocketServer(settings);
        }

        void run() {
            auto unsubscriber = _comunicator->subscribe(*this);

            _comunicator->run();
            
            unsubscriber->unsunscribe();
        }

        pm::Control operator()(const std::string& messageReceived)  {
            if(messageReceived == "exit") {
                std::cout << "exiting: " << std::endl;
                return pm::Control::Stop;
            }
            std::cout << "message received: " << messageReceived << std::endl;
            
            _comunicator->next(messageReceived);
            return pm::Control::Ok;
        }

        pm::Control next(const std::string& messageReceived) override { return this->operator()(messageReceived);}
        void complete() override { std::cout << "complete: " << std::endl; }
        ~Handler() {}
};

int main(int, char**) {
    Handler g;
    g.run();
}
