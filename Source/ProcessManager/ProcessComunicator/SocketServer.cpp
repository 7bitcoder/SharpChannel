#include <string>
#include "SocketServer.hpp"
#include "OsFlags.hpp"

#ifdef WIN

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

namespace pm {

    namespace {
        WSADATA wsaData;
        int iResult;

        SOCKET ListenSocket = INVALID_SOCKET;
        SOCKET ClientSocket = INVALID_SOCKET;

        struct addrinfo *result = NULL;
        struct addrinfo hints;

        int iSendResult;
        char* recvbuf;
    }

    SocketServer::~SocketServer() {
        delete[] recvbuf;
    }

    void SocketServer::init() {
        // Initialize Winsock
        iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
        if (iResult != 0) {
            printf("WSAStartup failed with error: %d\n", iResult);
            return;
        }

        recvbuf = new char[_settings.receiveBufferSize];
        
        if (recvbuf == nullptr) {
            return;
        }

        bool useTcp = _settings.socketType == SocketServerSettings::SocketType::Tcp;
        ZeroMemory(&hints, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol =  useTcp ? IPPROTO_TCP : IPPROTO_UDP;
        hints.ai_flags = AI_PASSIVE;

        // Resolve the server address and port
        auto portNumber = _settings.port;
        auto portNumberStr = std::to_string(portNumber);
        PCSTR port = (portNumber == 0) ? DEFAULT_PORT : TEXT(portNumberStr.c_str());
        iResult = getaddrinfo(NULL, port, &hints, &result);
        if ( iResult != 0 ) {
            printf("getaddrinfo failed with error: %d\n", iResult);
            WSACleanup();
            return;
        }

        // Create a SOCKET for connecting to server
        ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
        if (ListenSocket == INVALID_SOCKET) {
            //printf("socket failed with error: %ld\n", WSAGetLastError());
            freeaddrinfo(result);
            WSACleanup();
            return;
        }

        // Setup the TCP listening socket
        iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            printf("bind failed with error: %d\n", WSAGetLastError());
            freeaddrinfo(result);
            closesocket(ListenSocket);
            WSACleanup();
            return;
        }

        freeaddrinfo(result);
    }

    void SocketServer::run() {
        
        guard.lock();
        iResult = listen(ListenSocket, SOMAXCONN);
        if (iResult == SOCKET_ERROR) {
            printf("listen failed with error: %d\n", WSAGetLastError());
            closesocket(ListenSocket);
            WSACleanup();
            return;
        }

        // Accept a client socket
        ClientSocket = accept(ListenSocket, NULL, NULL);
        if (ClientSocket == INVALID_SOCKET) {
            printf("accept failed with error: %d\n", WSAGetLastError());
            closesocket(ListenSocket);
            WSACleanup();
            return;
        }

        // No longer need server socket
        closesocket(ListenSocket);
        guard.unlock();
        // Receive until the peer shuts down the connection
        do {

            iResult = recv(ClientSocket, recvbuf, _settings.receiveBufferSize, 0);
            if (iResult > 0) {
                printf("Bytes received: %d\n", iResult);
                recvbuf[iResult] = '\0';
                std::string msg(recvbuf);
                nextAll(msg);
            }
            else if (iResult == 0)
                printf("Connection closing...\n");
            else  {
                printf("recv failed with error: %d\n", WSAGetLastError());
                closesocket(ClientSocket);
                WSACleanup();
                return;
            }

        } while (iResult > 0);

        // shutdown the connection since we're done
        iResult = shutdown(ClientSocket, SD_SEND);
        if (iResult == SOCKET_ERROR) {
            printf("shutdown failed with error: %d\n", WSAGetLastError());
            closesocket(ClientSocket);
            WSACleanup();
            return;
        }

        // cleanup
        closesocket(ClientSocket);
        WSACleanup();

        completeAll();
        return;
    }

    bool SocketServer::sendImpl(const std::string& msg) {
        const std::lock_guard<std::mutex> lock(guard);
 
        iSendResult = ::send( ClientSocket, msg.c_str(), msg.length(), 0 );
        if (iSendResult == SOCKET_ERROR) {
            printf("send failed with error: %d\n", WSAGetLastError());
            closesocket(ClientSocket);
            WSACleanup();
            return false;
        }
        printf("Bytes sent: %d\n", iSendResult);
        return true;
    }
}
#endif