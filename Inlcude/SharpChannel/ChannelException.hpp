#pragma once
#include <iostream>

namespace cm
{
    class ChannelException : public std::exception
    {
    public:
        explicit ChannelException(const char *message)
            : _msg(message) {}

        explicit ChannelException(const std::string &message)
            : _msg(message) {}

        virtual ~ChannelException() noexcept {}

        virtual const char *what() const noexcept { return _msg.c_str(); }

    protected:
        std::string _msg;
    };
}