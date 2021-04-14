#include <memory>
#include <vector>
#include "SocketServerLinux.hpp"
#include "Settings.hpp"
#include "SharpChannel.hpp"
namespace cm
{

    std::unique_ptr<SocketServer> SocketServer::getObject(const SocketServerSettings &settings, IChannelEventLoop *eventLoop)
    {
        auto comunicator = std::make_unique<SocketServerLinux>(settings);
        comunicator->setChannelEventLoop(eventLoop);
        return comunicator;
    }

    SocketServerLinux::~SocketServerLinux()
    {
        delete[] recvbuf;
    }

    void SocketServerLinux::init()
    {
        recvbuf = new char[_settings.receiveBufferSize];
        if (recvbuf == nullptr)
        {
            throw cm::ChannelException("Failed To alocate buffer memory");
        }
        time_t ticks;

        bool useTcp = _settings.socketType == SocketServerSettings::SocketType::Tcp;
        listenfd = socket(AF_INET, SOCK_STREAM, useTcp ? IPPROTO_TCP : IPPROTO_UDP);
        if (listenfd == -1)
        {
            throw cm::ChannelException("Could not create socket");
        }
        memset(&serv_addr, '0', sizeof(serv_addr));

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        serv_addr.sin_port = htons(_settings.port);

        ;
        //Bind
        if (bind(listenfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        {
            throw cm::ChannelException("bind failed");
        }

        iResult = end ? -1 : listen(listenfd, 5);
        if (iResult == -1)
        {
            guard.unlock();
            throw cm::ChannelException("listen failed");
        }

        // Accept a client socket
        connfd = end ? -1 : accept(listenfd, (struct sockaddr *)&client, (socklen_t *)&c);
        if (connfd < 0)
        {
            guard.unlock();
            throw cm::ChannelException("accept failed");
        }

        // No longer need server socket
        close(listenfd);
    }

    void SocketServerLinux::run()
    {
        try
        {
            guard.lock();

            try
            {
                init();
            }
            catch (...)
            {
                guard.unlock();
                throw;
            }

            guard.unlock();
            // Receive until the peer shuts down the connection
            do
            {
                iResult = read(connfd, recvbuf, _settings.receiveBufferSize);
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
                    close(connfd);
                    throw cm::ChannelException("recv failed with error: ");
                }

            } while (iResult > 0);

            completeAll();
        }
        catch (std::exception &e)
        {
            errorAll(e);
            throw;
        }
        catch (...)
        {
            ChannelException e("generic Error ocured");
            errorAll(e);
            throw e;
        }
        delete[] recvbuf;
        recvbuf = nullptr;
    }

    bool SocketServerLinux::sendDataImpl(const std::vector<char> &data)
    {
        return sendRawData(data.data(), data.size());
    }

    bool SocketServerLinux::sendMessageImpl(const std::string &msg)
    {
        return sendRawData(msg.c_str(), msg.length());
    }

    bool SocketServerLinux::sendRawData(const char *data, const size_t lenght)
    {
        const char *data_ptr = (const char *)data;
        long int bytes_sent = 0;
        size_t data_size = lenght;

        while (data_size > 0)
        {
            bytes_sent = write(connfd, data_ptr, data_size);
            if (bytes_sent == -1)
                return false;

            data_ptr += bytes_sent;
            data_size -= bytes_sent;
        }

        return true;
    }

}