#pragma once
#include <mutex>
#include "IRunnable.hpp"
#include "Settings.hpp"
#include "Control.hpp"
#include "Channel.hpp"
#include "SocketServer.hpp"

namespace cm
{
    class SocketServerWin final : public SocketServer
    {
    public:
        SocketServerWin(const SocketServerSettings &settings)
        {
            _settings = settings;
            init();
        }
        virtual ~SocketServerWin();
        void init();
        void run() override;

        bool sendMessageImpl(const std::string &msg) override;
        bool sendDataImpl(const std::vector<char> &data) override;

    private:
        bool sendData(const char *data, const size_t lenght);
        std::mutex guard;
        SocketServerSettings _settings;
    };
}