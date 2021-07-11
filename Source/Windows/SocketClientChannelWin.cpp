#include <memory>
#include <vector>
#include "SocketClientChannelWin.hpp"
#include "Settings.hpp"
#include <windows.h>



// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

namespace cm
{

    SocketClientChannel::Ptr SocketClientChannel::create(const SocketClientSettings &settings, IChannelEventLoop *eventLoop)
    {
        auto comunicator = std::make_unique<SocketClientChannelWin>(settings);
        comunicator->setChannelEventLoop(eventLoop);
        return comunicator;
    }

    SocketClientChannelWin::~SocketClientChannelWin()
    {
        delete[] recvbuf;
    }

    void SocketClientChannelWin::init() {
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
            WSACleanup();
            throw cm::ChannelException(std::string("getaddrinfo failed with error: ") + std::to_string(WSAGetLastError()));
        }

        // Create a SOCKET for connecting to server
       for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

            // Create a SOCKET for connecting to server
            ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
                ptr->ai_protocol);
            if (ConnectSocket == INVALID_SOCKET) {
                WSACleanup();
                throw cm::ChannelException(std::string("socket failed with error: ") + std::to_string(WSAGetLastError()));
            }

            // Connect to server.
            iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
            if (iResult == SOCKET_ERROR) {
                closesocket(ConnectSocket);
                ConnectSocket = INVALID_SOCKET;
                continue;
            }
            break;
        }

        freeaddrinfo(result);
        if (ConnectSocket == INVALID_SOCKET) {
            WSACleanup();
            throw cm::ChannelException("Unable to connect to server");
        }
    }

    void SocketClientChannelWin::run()
    {
        try {
            guard.lock();
            try
            {
                init();
            }
            catch(...)
            {
                guard.unlock();
                throw;
            }
            
            guard.unlock();
            // Receive until the peer shuts down the connection
            do
            {
                iResult = recv(ConnectSocket, recvbuf, _settings.receiveBufferSize, 0);
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
                    closesocket(ConnectSocket);
                    WSACleanup();
                    throw cm::ChannelException(std::string("recv failed with error: ") + std::to_string(WSAGetLastError()));
                }

            } while (iResult > 0);

            // shutdown the connection since we're done
            iResult = shutdown(ConnectSocket, SD_SEND);
            if (iResult == SOCKET_ERROR)
            {
                closesocket(ConnectSocket);
                WSACleanup();
                throw cm::ChannelException(std::string("shutdown failed with error: ") + std::to_string(WSAGetLastError()));
            }

            // cleanup
            closesocket(ConnectSocket);
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

    bool SocketClientChannelWin::sendDataImpl(const std::vector<char> &data)
    {
        return sendRawData(data.data(), data.size());
    }

    bool SocketClientChannelWin::sendMessageImpl(const std::string &msg)
    {
        return sendRawData(msg.c_str(), msg.length());
    }

    bool SocketClientChannelWin::sendRawData(const char *data, const size_t lenght)
    {
        const char *data_ptr = (const char*) data;
        long int bytes_sent = 0;
        size_t data_size = lenght;

        while (data_size > 0)
        {
            bytes_sent = ::send(ConnectSocket, data_ptr, data_size, 0);
            if (bytes_sent == SOCKET_ERROR)
                return false;

            data_ptr += bytes_sent;
            data_size -= bytes_sent;
        }

        return true;
    }

}