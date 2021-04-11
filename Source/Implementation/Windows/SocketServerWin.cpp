#include <memory>
#include <vector>
#include "SocketServerWin.hpp"
#include "Settings.hpp"
namespace cm
{

    std::unique_ptr<SocketServer> SocketServer::getObject(const SocketServerSettings &settings, IChannelEventLoop *eventLoop)
    {
        auto comunicator = std::make_unique<SocketServerWin>(settings);
        comunicator->setChannelEventLoop(eventLoop);
        return comunicator;
    }

    SocketServerWin::~SocketServerWin()
    {
        delete[] recvbuf;
    }

    void SocketServerWin::init()
    {
        // Initialize Winsock
        iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (iResult != 0)
        {
            throw cm::ChannelException(std::string("WSAStartup failed with error: ") + std::to_string(iResult));
        }

        recvbuf = new char[_settings.receiveBufferSize];

        if (recvbuf == nullptr)
        {
            throw cm::ChannelException("Failed To alocate buffer memory");
        }

        bool useTcp = _settings.socketType == SocketServerSettings::SocketType::Tcp;
        ZeroMemory(&hints, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = useTcp ? IPPROTO_TCP : IPPROTO_UDP;
        hints.ai_flags = AI_PASSIVE;

        // Resolve the server address and port
        auto portNumber = _settings.port;
        auto portNumberStr = std::to_string(portNumber);
        PCSTR port = (portNumber == 0) ? DEFAULT_PORT : TEXT(portNumberStr.c_str());
        iResult = getaddrinfo(NULL, port, &hints, &result);
        if (iResult != 0)
        {
            WSACleanup();
            throw cm::ChannelException(std::string("getaddrinfo failed with error: ") + std::to_string(WSAGetLastError()));
        }

        // Create a SOCKET for connecting to server
        ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
        if (ListenSocket == INVALID_SOCKET)
        {
            freeaddrinfo(result);
            WSACleanup();
            throw cm::ChannelException(std::string("socket failed with error: ") + std::to_string(WSAGetLastError()));
        }

        // Setup the TCP listening socket
        iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
        if (iResult == SOCKET_ERROR)
        {
            freeaddrinfo(result);
            closesocket(ListenSocket);
            WSACleanup();
            throw cm::ChannelException(std::string("bind failed with error: ") + std::to_string(WSAGetLastError()));
        }

        freeaddrinfo(result);
    }

    void SocketServerWin::run()
    {
        try {
            init();
            guard.lock();
            iResult = listen(ListenSocket, SOMAXCONN);
            if (iResult == SOCKET_ERROR)
            {
                closesocket(ListenSocket);
                WSACleanup();
                throw cm::ChannelException(std::string("listen failed with error: ") + std::to_string(WSAGetLastError()));
            }

            // Accept a client socket
            ClientSocket = ::accept(ListenSocket, NULL, NULL);
            if (ClientSocket == INVALID_SOCKET)
            {
                closesocket(ListenSocket);
                WSACleanup();
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
        } catch (...) {
            errorAll(std::exception("generic Error ocured"));
        }
        delete[] recvbuf;
        recvbuf = nullptr;
    }

    bool SocketServerWin::sendDataImpl(const std::vector<char> &data)
    {
        return sendData(data.data(), data.size());
    }

    bool SocketServerWin::sendMessageImpl(const std::string &msg)
    {
        return sendData(msg.c_str(), msg.length());
    }

    bool SocketServerWin::sendData(const char *data, const size_t lenght)
    {
        iSendResult = ::send(ClientSocket, data, lenght, 0);
        if (iSendResult == SOCKET_ERROR)
        {
            // printf("send failed with error: %d\n", WSAGetLastError());
            closesocket(ClientSocket);
            WSACleanup();
            return false;
        }
        return true;
    }

}