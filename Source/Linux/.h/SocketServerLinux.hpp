#pragma once
#include <mutex>
#include <atomic>
#include "IRunnable.hpp"
#include "Settings.hpp"

#include "Channel.hpp"
#include "SocketServer.hpp"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

namespace cm {
    class SocketServerLinux final: public SocketServer {
        public:
            SocketServerLinux(const SocketServerSettings &settings) { _settings = settings; end = false; }
            ~SocketServerLinux();
            void init();
            void run() override;

            bool sendMessageImpl(const std::string &msg) override;
            bool sendDataImpl(const std::vector<char> &data) override;

            void finish() override {
                end = true;
                close(listenfd);
                close(connfd);
            }
        private:
            bool sendRawData(const char *data, const size_t lenght);
            std::mutex guard;
            SocketServerSettings _settings;
            std::atomic_bool end;
            
            int iResult;

            int listenfd = 0;
            int connfd = 0;
            int c = 0;
            struct sockaddr_in serv_addr;
            struct sockaddr_in client;

            char *recvbuf;
    };
}