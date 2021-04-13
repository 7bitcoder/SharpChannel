#include <memory>
#include <vector>
#include "SocketClientWin.hpp"
#include "Settings.hpp"#include <windows.h>



// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

namespace cm
{

    std::unique_ptr<SocketClient> SocketClient::getObject(const SocketClientSettings &settings, IChannelEventLoop *eventLoop)
    {
        auto comunicator = std::make_unique<SocketClientWin>(settings);
        comunicator->setChannelEventLoop(eventLoop);
        return comunicator;
    }

    SocketClientWin::~SocketClientWin()
    {
        delete[] recvbuf;
    }

    void SocketClientWin::run()
    {
        try {
            guard.lock();
            iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
            if (iResult != 0)
            {
                guard.unlock();
                throw cm::ChannelException(std::string("WSAStartup failed with error: ") + std::to_string(iResult));
            }

            recvbuf = new char[_settings.receiveBufferSize];

            if (recvbuf == nullptr)
            {
                guard.unlock();
                throw cm::ChannelException("Failed To alocate buffer memory");
            }

            bool useTcp = _settings.socketType == SocketClientSettings::SocketType::Tcp;
            ZeroMemory(&hints, sizeof(hints));
            hints.ai_family = AF_UNSPEC;
            hints.ai_socktype = SOCK_STREAM;
            hints.ai_protocol = useTcp ? IPPROTO_TCP : IPPROTO_UDP;

            // Resolve the server address and port
            auto portNumber = _settings.port;
            auto portNumberStr = std::to_string(portNumber);
            PCSTR port = (portNumber == 0) ? DEFAULT_PORT : TEXT(portNumberStr.c_str());
            iResult = getaddrinfo(_settings.address.c_str(), port, &hints, &result);
            if (iResult != 0)
            {
                guard.unlock();
                WSACleanup();
                throw cm::ChannelException(std::string("getaddrinfo failed with error: ") + std::to_string(WSAGetLastError()));
            }

            // Create a SOCKET for connecting to server
            ListenSocket = end ? INVALID_SOCKET : socket(result->ai_family, result->ai_socktype, result->ai_protocol);
            if (ListenSocket == INVALID_SOCKET)
            {
                guard.unlock();
                freeaddrinfo(result);
                WSACleanup();
                throw cm::ChannelException(std::string("socket failed with error: ") + std::to_string(WSAGetLastError()));
            }

            // Setup the TCP listening socket
            iResult = end ? SOCKET_ERROR : bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
            if (iResult == SOCKET_ERROR)
            {
                guard.unlock();
                freeaddrinfo(result);
                closesocket(ListenSocket);
                WSACleanup();
                throw cm::ChannelException(std::string("bind failed with error: ") + std::to_string(WSAGetLastError()));
            }

            freeaddrinfo(result);
            iResult = end ? SOCKET_ERROR : listen(ListenSocket, SOMAXCONN);
            if (iResult == SOCKET_ERROR)
            {
                closesocket(ListenSocket);
                WSACleanup();
                guard.unlock();
                throw cm::ChannelException(std::string("listen failed with error: ") + std::to_string(WSAGetLastError()));
            }

            // Accept a client socket
            ClientSocket = end ? INVALID_SOCKET : ::accept(ListenSocket, NULL, NULL);
            if (ClientSocket == INVALID_SOCKET)
            {
                closesocket(ListenSocket);
                WSACleanup();
                guard.unlock();
                throw cm::ChannelException(std::string("accept failed with error: ") + std::to_string(WSAGetLastError()));
            }

            // No longer need server socket
            closesocket(ListenSocket);
            guard.unlock();
            // Receive until the peer shuts down the connection
            do
            {
                iResult = recv(ClientSocket, recvbuf, _settings.receiveBufferSize, 0);
                if (iResult > 0)
                {
                    recvbuf[iResult] = '\0';
                    std::vector<char> vec(recvbuf, recvbuf + iResult);
                    nextAll(vec);
                }
                else if (iResult == 0)
                    break;
                else
                {
                    closesocket(ClientSocket);
                    WSACleanup();
                    throw cm::ChannelException(std::string("recv failed with error: ") + std::to_string(WSAGetLastError()));
                }

            } while (iResult > 0);

            // shutdown the connection since we're done
            iResult = shutdown(ClientSocket, SD_SEND);
            if (iResult == SOCKET_ERROR)
            {
                closesocket(ClientSocket);
                WSACleanup();
                throw cm::ChannelException(std::string("shutdown failed with error: ") + std::to_string(WSAGetLastError()));
            }

            // cleanup
            closesocket(ClientSocket);
            WSACleanup();

            completeAll();
        } catch(std::exception& e) {
            errorAll(e);
            throw;
        } catch (...) {
            ChannelException e("generic Error ocured");
            errorAll(e);
            throw e;
        }
        delete[] recvbuf;
        recvbuf = nullptr;
    }

    bool SocketClientWin::sendDataImpl(const std::vector<char> &data)
    {
        return sendRawData(data.data(), data.size());
    }

    bool SocketClientWin::sendMessageImpl(const std::string &msg)
    {
        return sendRawData(msg.c_str(), msg.length());
    }

    bool SocketClientWin::sendRawData(const char *data, const size_t lenght)
    {
        const char *data_ptr = (const char*) data;
        long int bytes_sent = 0;
        size_t data_size = lenght;

        while (data_size > 0)
        {
            bytes_sent = ::send(ClientSocket, data_ptr, data_size, 0);
            if (bytes_sent == SOCKET_ERROR)
                return false;

            data_ptr += bytes_sent;
            data_size -= bytes_sent;
        }

        return true;
    }

}