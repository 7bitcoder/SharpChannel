#pragma once
#include <mutex>
#include <atomic>
#include "Settings.hpp"
#include "Channels/INamedPipeChannel.hpp"
#include "ReadOnlyChannel.hpp"
#include "WriteOnlyChannel.hpp"

namespace cm
{
    class NamedPipeChannelWin final : public INamedPipeChannel, public ReadOnlyChannel, public WriteOnlyChannel
    {
    public:
        NamedPipeChannelWin(const NamedPipeChannelSettings &settings)
        {
            _settings = settings;
        }
        ~NamedPipeChannelWin();
        void run() final;

        bool sendMessageImpl(const std::string &msg) final;
        bool sendDataImpl(const std::vector<char> &data) final;
        void error(const std::exception&) final {}
        void complete() final {}

        void finish() final
        {
        }
        
    private:
        void init();
        NamedPipeChannelSettings _settings;
    };
}