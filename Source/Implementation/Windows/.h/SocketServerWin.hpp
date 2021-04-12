#pragma once
#include <mutex>
#include <atomic>
#include "IRunnable.hpp"
#include "Settings.hpp"
#include "Channel.hpp"
#include "SocketServer.hpp"
#include "SharpChannel.hpp"

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"


namespace cm
{
    class SocketServerWin final : public SocketServer
    {
    public:
        SocketServerWin(const SocketServerSettings &settings) { _settings = settings; end = false; }
        virtual ~SocketServerWin();
        void init();
        void run() override;

        bool sendMessageImpl(const std::string &msg) override;
        bool sendDataImpl(const std::vector<char> &data) override;

        void finish() override {
            end = true;
            closesocket(ListenSocket);
            closesocket(ClientSocket);
            WSACleanup();
        }

    private:
        bool sendRawData(const char *data, const size_t lenght);
        std::mutex guard;
        std::atomic_bool end;
        SocketServerSettings _settings;

        WSADATA wsaData;
        int iResult;

        SOCKET ListenSocket = INVALID_SOCKET;
        SOCKET ClientSocket = INVALID_SOCKET;

        struct addrinfo *result = NULL;
        struct addrinfo hints;

        int iSendResult;
        char *recvbuf;
    };
}