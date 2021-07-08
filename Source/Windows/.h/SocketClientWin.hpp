#pragma once
#include <mutex>
#include <atomic>
#include "IRunnable.hpp"
#include "Settings.hpp"
#include "SocketClient.hpp"
#include "SharpChannel.hpp"

#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

namespace cm
{
    class SocketClientWin final : public SocketClient
    {
    public:
        SocketClientWin(const SocketClientSettings &settings)
        {
            _settings = settings;
            end = false;
        }
        ~SocketClientWin();
        void run() override;

        bool sendMessageImpl(const std::string &msg) override;
        bool sendDataImpl(const std::vector<char> &data) override;

        void finish() override
        {
            end = true;
            closesocket(ConnectSocket);
            WSACleanup();
        }

    private:
        void init();
        bool sendRawData(const char *data, const size_t lenght);
        std::mutex guard;
        std::atomic_bool end;
        SocketClientSettings _settings;

        WSADATA wsaData;
        SOCKET ConnectSocket = INVALID_SOCKET;

        struct addrinfo *result = NULL;
        struct addrinfo *ptr = NULL;
        struct addrinfo hints;

        int iResult;
        int recvbuflen = DEFAULT_BUFLEN;
        char *recvbuf;
    };
}