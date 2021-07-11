#pragma once
#include <mutex>
#include <atomic>
#include "Settings.hpp"
#include "Channels/ISocketClientChannel.hpp"
#include "ReadOnlyChannel.hpp"
#include "WriteOnlyChannel.hpp"

#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

namespace cm
{
    class SocketClientChannelWin final : public ISocketClientChannel, public ReadOnlyChannel, public WriteOnlyChannel
    {
    public:
        SocketClientChannelWin(const SocketClientSettings &settings)
        {
            _settings = settings;
            end = false;
        }
        ~SocketClientChannelWin();
        void run() final;

        bool sendMessageImpl(const std::string &msg) final;
        bool sendDataImpl(const std::vector<char> &data) final;
        void error(const std::exception&) final {}
        void complete() final {}

        void finish() final
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